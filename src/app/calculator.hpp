#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "task.hpp"

namespace math {
enum class StatusCode;
} // namespace math

namespace calculator {

class Calculator {
public:
    Calculator();
    ~Calculator();

    Calculator(const Calculator& other);
    Calculator& operator=(const Calculator& other);

    Calculator(Calculator&& other) noexcept;
    Calculator& operator=(Calculator&& other) noexcept;

    void calculate(Task& task) const;

private:
    void initialize();

    using BinaryOp = std::function<math::StatusCode(int, int, int&)>;
    using UnaryOp = std::function<math::StatusCode(int, int&)>;

    std::unordered_map<std::string, BinaryOp> binary_ops_;
    std::unordered_map<std::string, UnaryOp> unary_ops_;
};

} // namespace calculator
