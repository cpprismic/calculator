#include "app.hpp"

#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "math.hpp"

bool app::parse(int argc, char** argv, app::Task& task) {
    int opt = 0;

    optind = 1;

    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h':
            printHelp();
            exit(0);
        case '?':
            fprintf(stderr, "Error: unknown option. Use -h for help.\n");
            return false;
        default:
            fprintf(stderr, "Error: unexpected option.\n");
            return false;
        }
    }

    const int count = argc - optind;

    if (count == 0) {
        fprintf(stderr, "Error: no arguments were passed. Use -h for help.\n");
        return false;
    }

    const char* op_str = argv[optind + count - 1];
    if (strlen(op_str) != 1) {
        fprintf(stderr, "Error: the operator must be a single character. Use -h for help.\n");
        return false;
    }
    task.operation = op_str[0];

    if (task.operation == '!') {
        if (count != app::UNARY_ARGS_COUNT) {
            fprintf(stderr, "Error: 1 number is required for the '!' operator. Use -h for help.\n");
            return false;
        }
        char* endptr = nullptr;
        errno = 0;
        const long val = strtol(argv[optind], &endptr, 10);
        if (errno != 0 || endptr == argv[optind] || *endptr != '\0' || val < INT_MIN || val > INT_MAX) {
            fprintf(stderr, "Error: '%s' is not a valid integer.\n", argv[optind]);
            return false;
        }
        task.value1 = static_cast<int>(val);
        task.value2 = 0;
    } else {
        if (count != app::BINARY_ARGS_COUNT) {
            fprintf(stderr, "Error: 2 numbers and an operation are required. Use -h for help.\n");
            return false;
        }
        char* endptr = nullptr;
        errno = 0;
        const long val1 = strtol(argv[optind], &endptr, 10);
        if (errno != 0 || endptr == argv[optind] || *endptr != '\0' || val1 < INT_MIN || val1 > INT_MAX) {
            fprintf(stderr, "Error: '%s' is not a valid integer.\n", argv[optind]);
            return false;
        }
        task.value1 = static_cast<int>(val1);

        endptr = nullptr;
        errno = 0;
        const long val2 = strtol(argv[optind + 1], &endptr, 10);
        if (errno != 0 || endptr == argv[optind + 1] || *endptr != '\0' || val2 < INT_MIN || val2 > INT_MAX) {
            fprintf(stderr, "Error: '%s' is not a valid integer.\n", argv[optind + 1]);
            return false;
        }
        task.value2 = static_cast<int>(val2);
    }

    return true;
}

bool app::check(const app::Task& task) {
    if (task.operation != '+' && task.operation != '-' && task.operation != '*' && task.operation != '/' &&
        task.operation != '^' && task.operation != '!') {
        return false;
    }

    if (task.operation == '^' && task.value2 < 0) {
        return false;
    }

    return true;
}

bool app::calculate(app::Task& task) {
    math::StatusCode code = math::StatusCode::SUCCESS;

    switch (task.operation) {
    case '+':
        code = math::add(task.value1, task.value2, task.result);
        break;
    case '-':
        code = math::subtract(task.value1, task.value2, task.result);
        break;
    case '*':
        code = math::multiply(task.value1, task.value2, task.result);
        break;
    case '/':
        code = math::divide(task.value1, task.value2, task.result);
        break;
    case '^':
        code = math::power(task.value1, task.value2, task.result);
        break;
    case '!':
        code = math::factorial(task.value1, task.result);
        break;
    default:
        code = math::StatusCode::INVALID_ARGUMENT;
        break;
    }

    if (code != math::StatusCode::SUCCESS) {
        fprintf(stderr, "Error: %s\n", math::statusCodeToString(code));
        return false;
    }

    return true;
}

void app::printResult(const app::Task& task) {
    if (task.operation == '!') {
        printf("%d! = %d\n", task.value1, task.result);
    } else {
        printf("%d %c %d = %d\n", task.value1, task.operation, task.value2, task.result);
    }
}

void app::printHelp() {
    printf("Usage:\n");
    printf("  calculator <num1> <num2> <op>        Binary operation\n");
    printf("  calculator <num> !                   Unary factorial\n\n");
    printf("Binary operations:\n");
    printf("  +  addition\n");
    printf("  -  subtraction\n");
    printf("  *  multiplication\n");
    printf("  /  division (integer division)\n");
    printf("  ^  exponentiation\n\n");
    printf("Unary operation:\n");
    printf("  !  factorial (for non-negative integers)\n\n");
    printf("Examples:\n");
    printf("  calculator 10 5 +\n");
    printf("  calculator 15 3 *\n");
    printf("  calculator 15 3 ^\n");
    printf("  calculator 5 !\n");
}

void app::run(int argc, char** argv) {
    app::Task task = {};
    if (!parse(argc, argv, task)) {
        exit(1);
    }
    if (!check(task)) {
        fprintf(stderr, "Error: unknown operator '%c'.\n", task.operation);
        exit(1);
    }
    if (!calculate(task)) {
        exit(1);
    }
    printResult(task);
}
