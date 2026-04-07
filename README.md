# Calculator

Консольный калькулятор с JSON-интерфейсом. Принимает операцию и операнды в виде JSON-строки, выводит результат.

## Требования

- CMake 3.16+
- clang-tidy
- clang-format
- Git (для загрузки зависимостей)

## Сборка

### Debug (по умолчанию)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

В Debug-сборке логгер устанавливает уровень **debug** — в консоль выводятся все сообщения, включая подробный трейс разбора JSON и вычислений.

### Release

```bash
cmake -S . -B build_release -DCMAKE_BUILD_TYPE=Release
cmake --build build_release
```

В Release-сборке логгер устанавливает уровень **info** — выводятся только информационные сообщения, предупреждения и ошибки.

## Запуск

```bash
./calculator '<json>'
```

### Поддерживаемые операции

#### Бинарные (2 операнда)

```bash
# Сложение
./calculator '{"operation": "add", "operands": [2, 3]}'

# Вычитание
./calculator '{"operation": "subtract", "operands": [10, 4]}'

# Умножение
./calculator '{"operation": "multiply", "operands": [6, 7]}'

# Деление
./calculator '{"operation": "divide", "operands": [20, 4]}'

# Возведение в степень
./calculator '{"operation": "power", "operands": [2, 10]}'
```

#### Унарные (1 операнд)

```bash
# Факториал
./calculator '{"operation": "factorial", "operands": [5]}'
```

## Тесты

```bash
cmake --build build && ctest --test-dir build
```

Запуск конкретного теста:

```bash
./calculator_tests --gtest_filter="CalculatorTest.Add"
```

## Логи

Помимо вывода в консоль, все сообщения сохраняются в файл `logs/app.log` (директория создаётся автоматически при первом запуске).
