#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "task.hpp"

namespace app {

class Calculator {
public:
    Calculator();

    int calculate(const Task& task) const;

private:
    void initialize();

    using BinaryOp = std::function<int(int, int)>;
    using UnaryOp = std::function<int(int)>;

    std::unordered_map<std::string, BinaryOp> binary_ops_;
    std::unordered_map<std::string, UnaryOp> unary_ops_;
};

} // namespace app