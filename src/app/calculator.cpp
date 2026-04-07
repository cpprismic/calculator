#include "calculator.hpp"

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
    for (std::size_t i = 0; i < task.operands.size(); ++i) {
        log.debug("Operand[" + std::to_string(i) + "] = " + std::to_string(task.operands[i]));
    }

    int result = 0;
    if (const auto it = binary_ops_.find(task.operation); it != binary_ops_.end()) {
        result = it->second(task.operands[0], task.operands[1]);
    } else if (const auto it = unary_ops_.find(task.operation); it != unary_ops_.end()) {
        result = it->second(task.operands[0]);
    } else {
        // Сюда не должны добраться - Parser уже валидировал операцию
        throw CalculationException("Unknown operation: " + task.operation);
    }

    log.debug("Result: " + std::to_string(result));
    return result;
}

void Calculator::initialize() {
    binary_ops_["add"] = [](int a, int b) {
        int result = 0;
        switch (math::add(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw CalculationException("Overflow in addition");
        default:
            throw CalculationException("Unknown error in addition");
        }
    };
    binary_ops_["subtract"] = [](int a, int b) {
        int result = 0;
        switch (math::subtract(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw CalculationException("Overflow in subtraction");
        default:
            throw CalculationException("Unknown error in subtraction");
        }
    };
    binary_ops_["multiply"] = [](int a, int b) {
        int result = 0;
        switch (math::multiply(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw CalculationException("Overflow in multiplication");
        default:
            throw CalculationException("Unknown error in multiplication");
        }
    };
    binary_ops_["divide"] = [](int a, int b) {
        int result = 0;
        switch (math::divide(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::DIVISION_BY_ZERO:
            throw CalculationException("Division by zero");
        case math::StatusCode::OVERFLOW:
            throw CalculationException("Overflow in division");
        default:
            throw CalculationException("Unknown error in division");
        }
    };
    binary_ops_["power"] = [](int a, int b) {
        int result = 0;
        switch (math::power(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw CalculationException("Overflow in power");
        case math::StatusCode::INVALID_ARGUMENT:
            throw CalculationException("Negative exponent is not supported");
        default:
            throw CalculationException("Unknown error in power");
        }
    };

    unary_ops_["factorial"] = [](int a) {
        int result = 0;
        switch (math::factorial(a, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw CalculationException("Overflow in factorial");
        case math::StatusCode::INVALID_ARGUMENT:
            throw CalculationException("Factorial of negative number");
        default:
            throw CalculationException("Unknown error in factorial");
        }
    };
}

} // namespace app