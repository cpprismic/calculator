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
bool parse(int argc, char** argv, Task& task);
bool check(const Task& task);
bool calculate(Task& task);
void printResult(const Task& task);
void printHelp();

} // namespace app