#pragma once

namespace app {

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