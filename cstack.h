#ifndef CSTACK_H
#define CSTACK_H

/* Stack handler: an abstract identifier, not a pointer to stack data. */
typedef int hstack_t;

/* Create a new stack; returns its handler or -1 on error. */
hstack_t stack_new(void);

/* Remove the stack by handler, if it exists. */
void stack_free(const hstack_t stack);

/* Check handler validity; returns 0 if the stack exists, 1 otherwise. */
int stack_valid_handler(const hstack_t stack);

/* Get the element count; returns 0 if the stack does not exist. */
unsigned int stack_size(const hstack_t stack);

/* Push a data element from a buffer onto the stack. */
void
stack_push(const hstack_t stack, const void* data_in, const unsigned int size);

/* Pop a data element into a buffer; returns written bytes or 0 on failure. */
unsigned int
stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

/* Create a new stack filled with handlers of all other stacks;
 * returns its handler or -1 on error, leaving existing stacks unchanged. */
hstack_t stack_get_all(void);

#endif /* CSTACK_H */
