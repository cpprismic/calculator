#include "calculator.hpp"

#include <climits>

#include "../logger/logger.hpp"
#include "exceptions.hpp"
#include "math.hpp"

namespace app {

Calculator::Calculator() {
    initialize();
}

int Calculator::calculate(const Task& task) const {
    auto& log = logger::Logger::getInstance();
    log.debug("Calculating: " + task.operation);

    if (const auto it = binary_ops_.find(task.operation); it != binary_ops_.end()) {
        return it->second(task.operands[0], task.operands[1]);
    }
    if (const auto it = unary_ops_.find(task.operation); it != unary_ops_.end()) {
        return it->second(task.operands[0]);
    }

    // Сюда не должны добирться - Parser уже валидировал операцию
    throw CalculationException("Unknown operation: " + task.operation);
}

void Calculator::initialize() {
    binary_ops_["add"] = [](int a, int b) {
        int result = 0;
        if (math::add(a, b, result) != math::StatusCode::SUCCESS) {
            throw CalculationException("Overflow in addition");
        }
        return result;
    };
    binary_ops_["subtract"] = [](int a, int b) {
        int result = 0;
        if (math::subtract(a, b, result) != math::StatusCode::SUCCESS) {
            throw CalculationException("Overflow in subtraction");
        }
        return result;
    };
    binary_ops_["multiply"] = [](int a, int b) {
        int result = 0;
        if (math::multiply(a, b, result) != math::StatusCode::SUCCESS) {
            throw CalculationException("Overflow in multiplication");
        }
        return result;
    };
    binary_ops_["divide"] = [](int a, int b) {
        if (b == 0) {
            throw CalculationException("Division by zero");
        }
        int result = 0;
        math::divide(a, b, result);
        return result;
    };
    binary_ops_["power"] = [](int a, int b) {
        if (b < 0) {
            throw CalculationException("Negative exponent is not supported");
        }
        int result = 0;
        if (math::power(a, b, result) != math::StatusCode::SUCCESS) {
            throw CalculationException("Overflow in power");
        }
        return result;
    };

    unary_ops_["factorial"] = [](int a) {
        if (a < 0) {
            throw CalculationException("Factorial of negative number");
        }
        int result = 0;
        if (math::factorial(a, result) != math::StatusCode::SUCCESS) {
            throw CalculationException("Overflow in factorial");
        }
        return result;
    };
}

} // namespace app