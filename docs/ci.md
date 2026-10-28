# Работа с GitHub Actions и статическим анализом

## Пайплайн CI

В репозитории настроен [GitHub Actions](https://docs.github.com/ru/actions) — сервис непрерывной
интеграции, который автоматически собирает проект и запускает к нему проверки.
Описание workflow находится в файле [ci.yml](../.github/workflows/ci.yml).
Структура workflow:

* `format` — проверка автоформатирования файла `cstack.c` утилитой
  [clang-format](https://clang.llvm.org/docs/ClangFormat.html).
* `build`:
  - Сборка проекта
  - Статический анализ проекта утилитой [cppcheck](https://sourceforge.net/projects/cppcheck/)
  - Статический анализ проекта утилитой [clang-tidy](https://sourceforge.net/projects/clang-tidy/)
  - Запуск юнит-теста

Полный лог выполнения можно посмотреть на вкладке `Actions` вашего форка.

## Статический анализ

В workflow статическому анализу подвергается только файл `cstack.c`, тесты не проверяются.
Срабатывания статических анализаторов не прервут выполнение workflow.

### Подавление ложных срабатываний

Анализатор может ошибочно пометить корректный код (ложное срабатывание, false positive).
Если вы уверены, что срабатывание ложное, его можно подавить комментарием прямо в коде.

Подавляйте только действительно ложные срабатывания, по возможности указывая имя чекера и краткое
объяснение причины. Избыточное подавление диагностик будет считаться замечанием в процессе ревью.

**clang-tidy** использует комментарии `NOLINT`.

Пример:

```c
const unsigned int MAX_STACKS = 10u;  // NOLINT(readability-magic-numbers)
```

Полный синтаксис NOLINT описан
[в документации clang-tidy](https://clang.llvm.org/extra/clang-tidy.html#controlling-the-checks-configuration).

**cppcheck** использует комментарии вида `// cppcheck-suppress`.

Пример:

```c
int x = 42;  // cppcheck-suppress constVariable
```
