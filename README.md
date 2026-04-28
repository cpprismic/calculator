# Calculator

Консольный калькулятор с JSON-интерфейсом. Принимает операцию и операнды в виде JSON-строки, выводит результат.

Результаты операций сохраняются в PostgreSQL и кэшируются в памяти: повторные вычисления не выполняются — результат возвращается из кэша.

## Требования

- CMake 3.16+
- C++17
- clang-tidy
- clang-format
- Git
- libpq (PostgreSQL)

## Быстрый старт

### 1. Настройка базы данных

Скрипт создаёт пользователя, базу данных, выдаёт права и генерирует `config.json` в build-директории проекта:

```bash
./scripts/setup_db.sh
```

Параметры по умолчанию: пользователь `calc_user`, БД `calculator`, пароль `calc_password`.
Переопределяются флагами:

```bash
./scripts/setup_db.sh -d mydb -u myuser -w mysecret

# Доступные флаги:
#   -h HOST      хост PostgreSQL  (по умолчанию: localhost)
#   -p PORT      порт             (по умолчанию: 5432)
#   -d DBNAME    имя БД           (по умолчанию: calculator)
#   -u USER      пользователь     (по умолчанию: calc_user)
#   -w PASSWORD  пароль           (по умолчанию: calc_password)
```

### 2. Конфигурация

Скрипт создаёт `config.json` автоматически. Файл не коммитится в репозиторий — при необходимости создайте его вручную по шаблону `config.json.example`:

```json
{
    "database": {
        "host": "localhost",
        "port": 5432,
        "dbname": "calculator",
        "user": "calc_user",
        "password": "calc_password"
    }
}
```

Приложение ищет `config.json` в рабочей директории, из которой запускается. Запускайте бинарник из корня проекта, где лежит `config.json`:

```bash
./build/calculator '{"operation": "add", "operands": [2, 3]}'
```

Если `config.json` отсутствует или БД недоступна, приложение работает в режиме **только кэш** — результаты хранятся в памяти до завершения процесса.

### 3. Сборка

#### Debug (по умолчанию)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

В Debug-сборке логгер устанавливает уровень **debug** — в консоль выводятся все сообщения, включая трейс разбора JSON и вычислений.

#### Release

```bash
cmake -S . -B build_release -DCMAKE_BUILD_TYPE=Release
cmake --build build_release
```

В Release-сборке логгер устанавливает уровень **info** — выводятся только информационные сообщения, предупреждения и ошибки.

## Использование

```bash
./build/calculator '<json>'
```

### Поддерживаемые операции

#### Бинарные (2 операнда)

```bash
./build/calculator '{"operation": "add",      "operands": [2, 3]}'
./build/calculator '{"operation": "subtract", "operands": [10, 4]}'
./build/calculator '{"operation": "multiply", "operands": [6, 7]}'
./build/calculator '{"operation": "divide",   "operands": [20, 4]}'
./build/calculator '{"operation": "power",    "operands": [2, 10]}'
```

#### Унарные (1 операнд)

```bash
./build/calculator '{"operation": "factorial", "operands": [5]}'
```

## Кэш и хранение

При запуске приложение загружает историю операций из БД в кэш. Каждый последующий запрос сначала проверяется по кэшу:

- **Попадание** — результат возвращается без вычислений.
- **Промах** — операция выполняется, результат записывается в кэш и в БД.

Коммутативные операции (`add`, `multiply`) нормализуются: `add(2, 3)` и `add(3, 2)` дают один и тот же ключ кэша.

Схема таблицы в PostgreSQL:

```sql
CREATE TABLE calculations (
    id         SERIAL PRIMARY KEY,
    operation  VARCHAR(32) NOT NULL,
    first_num  INTEGER     NOT NULL,
    second_num INTEGER     NOT NULL,
    result     INTEGER     NOT NULL,
    status     INTEGER     NOT NULL  -- 0: успех, 1: ошибка, 2: деление на ноль, 3: переполнение буфера
);
```

## Тесты

```bash
cmake --build build && ctest --test-dir build
```

Запуск конкретного теста:

```bash
./build/calculator_tests --gtest_filter="CalculatorTest.Add"
```

Тесты не требуют подключения к БД.

## Valgrind

Проверка на утечки памяти встроена в тестовый прогон — тест `ValgrindMemcheck` запускается автоматически вместе с остальными:

```bash
ctest --test-dir build --output-on-failure
```

Для ручной проверки конкретного сценария:

```bash
# Успешная операция
valgrind --leak-check=full --show-leak-kinds=definite,indirect --error-exitcode=1 \
    ./build/calculator '{"operation": "add", "operands": [2, 3]}'

# Error-путь (деление на ноль)
valgrind --leak-check=full --show-leak-kinds=definite,indirect --error-exitcode=1 \
    ./build/calculator '{"operation": "divide", "operands": [10, 0]}'
```

`--show-leak-kinds=definite,indirect` исключает `still reachable` — глобальные буферы spdlog и libpq освобождаются при выходе из процесса и утечками не являются.

## Perf

Для профилирования нужна сборка с оптимизациями и отладочными символами:

```bash
cmake -S . -B build_perf -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build_perf
```

### Общая статистика

```bash
perf stat -r 100 ./build_perf/calculator '{"operation": "add", "operands": [2, 3]}'
```

### Профиль вызовов

```bash
perf record -g --call-graph dwarf \
    -- sh -c 'for i in $(seq 10000); do
        ./build_perf/calculator "{\"operation\": \"add\", \"operands\": [2, 3]}"
    done'

perf report
```

Для профилирования тестового бинаря (содержит тысячи операций, не требует цикла):

```bash
perf record -g --call-graph dwarf ./build_perf/calculator_tests
perf report
```

### Flame Graph

```bash
git clone --depth=1 https://github.com/brendangregg/FlameGraph /tmp/flamegraph

perf record -F 99 -g --call-graph dwarf ./build_perf/calculator_tests
perf script | /tmp/flamegraph/stackcollapse-perf.pl | \
    /tmp/flamegraph/flamegraph.pl > flamegraph.svg
```

Открыть `flamegraph.svg` в браузере — широкие блоки соответствуют узким местам.

## Логи

Помимо вывода в консоль, все сообщения сохраняются в `logs/app.log` (директория создаётся автоматически при первом запуске).
