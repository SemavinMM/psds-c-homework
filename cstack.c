#include "cstack.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct node
{
    struct node* prev;
    unsigned int size;
    char data[];
};

typedef struct node* stack_t;

struct stack_entry
{
    int reserved;
    stack_t stack;
};

typedef struct stack_entry stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

#define STACKS_CAPACITY 100u

static stack_entry_t g_entries[STACKS_CAPACITY];

static struct stack_entries_table g_table = {STACKS_CAPACITY, g_entries};

#define UNUSED(VAR) (void)(VAR)

hstack_t stack_new(void)
{
    unsigned int index;
    for (index = 0; index < g_table.size; ++index)
    {
        if (g_table.entries[index].reserved == 0)
        {
            g_table.entries[index].reserved = 1;
            g_table.entries[index].stack = NULL;

            return (hstack_t)index;
        }
    }

    return -1;
}

void stack_free(const hstack_t hstack)
{
    struct node* current;
    struct node* prev;

    if (!stack_valid_handler(hstack))
    {
        return;
    }

    current = g_table.entries[hstack].stack;
    while (current != NULL)
    {
        prev = current->prev;
        free(current);
        current = prev;
    }

    g_table.entries[hstack].reserved = 0;
    g_table.entries[hstack].stack = NULL;
}

void stack_free_all(void)
{
    unsigned int index;
    for (index = 0; index < g_table.size; ++index)
    {
        if (g_table.entries[index].reserved != 0)
        {
            stack_free((hstack_t)index);
        }
    }
}

int stack_valid_handler(const hstack_t hstack)
{
    if (hstack < 0)
    {
        return 0;
    }

    if ((unsigned int)hstack >= g_table.size)
    {
        return 0;
    }

    return g_table.entries[hstack].reserved != 0;
}

unsigned int stack_size(const hstack_t hstack)
{
    unsigned int count = 0;
    const struct node* current;

    if (!stack_valid_handler(hstack))
    {
        return 0;
    }
    current = g_table.entries[hstack].stack;

    while (current != NULL)
    {
        ++count;
        current = current->prev;
    }

    return count;
}

void
stack_push(const hstack_t hstack, const void* data_in, const unsigned int size)
{
    struct node* new_node;

    if (!stack_valid_handler(hstack) || data_in == NULL || size == 0)
    {
        return;
    }

#if SIZE_MAX <= UINT_MAX
    if (size > SIZE_MAX - sizeof(struct node))
    {
        return;
    }
#endif

    new_node = malloc(sizeof(struct node) + size);

    if (new_node == NULL)
    {
        return;
    }
    new_node->prev = g_table.entries[hstack].stack;
    new_node->size = size;
    memcpy(new_node->data, data_in, size);
    g_table.entries[hstack].stack = new_node;
}

unsigned int
stack_pop(const hstack_t hstack, void* data_out, const unsigned int size)
{
    struct node* top;

    if (!stack_valid_handler(hstack) || data_out == NULL || size == 0)
    {
        return 0;
    }

    top = g_table.entries[hstack].stack;
    unsigned int data_size;
    if (top == NULL || size < top->size)
    {
        return 0;
    }
    data_size = top->size;
    memcpy(data_out, top->data, data_size);
    g_table.entries[hstack].stack = (stack_t)top->prev;
    free(top);
    return data_size;
}

unsigned stack_top_size(const hstack_t hstack)
{
    const struct node* top;

    if (!stack_valid_handler(hstack))
    {
        return 0;
    }

    top = g_table.entries[hstack].stack;

    if (top == NULL)
    {
        return 0;
    }
    return top->size;
}

hstack_t stack_get_all(void)
{
    hstack_t new_stack;
    hstack_t handler;
    unsigned int index;
    unsigned int old_size;
    new_stack = stack_new();

    if (new_stack < 0)
    {
        return -1;
    }

    for (index = 0; index < g_table.size; ++index)
    {
        if (g_table.entries[index].reserved != 0 &&
            (hstack_t)index != new_stack)
        {
            handler = (hstack_t)index;
            old_size = stack_size(new_stack);
            stack_push(new_stack, &handler, sizeof(handler));

            if (stack_size(new_stack) != old_size + 1)
            {
                stack_free(new_stack);
                return -1;
            }
        }
    }
    return new_stack;
}
