#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>

#include "app.hpp"
#include "math.hpp"

void app::parse(int argc, char** argv, app::Task& task) {
    int opt;
    
    optind = 1;
    
    while ((opt = getopt(argc, argv, "h")) != -1) {
        switch (opt) {
        case 'h':
            printHelp(); 
            exit(0);
        case '?': 
            printf("Unknown option. Use -h for help.\n");
            exit(1);
        default: 
            exit(1);
        }
    }

    int count = argc - optind;

    if (count == 0) {
        printf("Error: No arguments were passed.\n");
        printf("Use -h for help.\n");
        exit(1);
    }

    const char* op_str = argv[optind + count - 1];
    if (strlen(op_str) != 1) {
        printf("Error: the operator must be a single character.\n");
        printf("Use -h for help.\n");
        exit(1);
    }
    task.operation = op_str[0];

    if (task.operation == '!') {
        if (count != 2) {
            printf("Error: 1 number is required for the '!' operator.\n");
            printf("Use -h for help.\n");
            exit(1);
        }
        task.value1 = atoi(argv[optind]);
        task.value2 = 0;
    } else {
        if (count != 3) {
            printf("Error: 2 numbers and an operation are required.\n");
            printf("Use -h for help.\n");
            exit(1);
        }
        task.value1 = atoi(argv[optind]);
        task.value2 = atoi(argv[optind + 1]);
    }

    task.result = 0;
}

bool app::check(app::Task task) {
    if (task.operation != '+' &&
        task.operation != '-' &&
        task.operation != '*' &&
        task.operation != '/' &&
        task.operation != '^' &&
        task.operation != '!') {
        return false;
    }

    if (task.operation == '/') {
        if (task.value2 == 0) {
            return false;
        }
    }

    if (task.operation == '!') {
        if (task.value1 < 0) {
            return false;
        }
    }

    return true;
}

void app::calculate(app::Task& task) {
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
        printf("Error: %s\n", math::statusCodeToString(code));
        exit(1);
    }
}

void app::printResult(app::Task task) {
    if (task.operation == '!') {
        printf("%d! = %d\n", task.value1, task.result);
    } else {
        printf("%d %c %d = %d\n", task.value1, task.operation, task.value2, task.result);
    }
}

void app::printHelp() {
    printf("Usage:\n");
    printf("  calculator <num1> <num2> <op>        Binary operation\n");
    printf("  calculator <num> !                    Unary factorial\n\n");
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
	app::Task task;
	app::parse(argc, argv, task);
    if (!check(task)) {
        printf("Error: Incorrect data or operation.\n");
        exit(1);
    }
	calculate(task);
	printResult(task);
}