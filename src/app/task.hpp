#pragma once

#include <string>

namespace calculator {

enum class OperationStatus { NotExecuted, Success, Failed };

struct Task {
    int first_number {0};
    int second_number {0};
    std::string operation;
    int result {0};
    OperationStatus status {OperationStatus::NotExecuted};
};

} // namespace calculator
