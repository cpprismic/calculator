#pragma once

namespace app {

constexpr int UNARY_ARGS_COUNT = 2;
constexpr int BINARY_ARGS_COUNT = 3;

struct Task {
    int value1;
    int value2;
    char operation;
    int result;
};

void run(int argc, char** argv);
void parse(int argc, char** argv, Task& task);
bool check(Task task);
void calculate(Task& task);
void printResult(Task task);
void printHelp();

} // namespace app