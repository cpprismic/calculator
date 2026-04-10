#include "calculator.hpp"

#include <stdexcept>

#include "../logger/logger.hpp"
#include "math.hpp"

namespace app {

Calculator::Calculator() {
    initialize();
}

Calculator::~Calculator() = default;

Calculator::Calculator(const Calculator&) = default;
Calculator& Calculator::operator=(const Calculator&) = default;

Calculator::Calculator(Calculator&&) noexcept = default;
Calculator& Calculator::operator=(Calculator&&) noexcept = default;

void Calculator::calculate(Task& task) const {
    auto& log = logger::Logger::getInstance();
    log.debug("Calculating: " + task.operation);
    log.debug("First number: " + std::to_string(task.first_number));

    try {
        if (const auto it = binary_ops_.find(task.operation); it != binary_ops_.end()) {
            log.debug("Second number: " + std::to_string(task.second_number));
            task.result = it->second(task.first_number, task.second_number);
        } else if (const auto it = unary_ops_.find(task.operation); it != unary_ops_.end()) {
            task.result = it->second(task.first_number);
        } else {
            task.status = OperationStatus::Failed;
            throw std::runtime_error("Unknown operation: " + task.operation);
        }
    } catch (const std::runtime_error&) {
        task.status = OperationStatus::Failed;
        throw;
    }

    task.status = OperationStatus::Success;
    log.debug("Result: " + std::to_string(task.result));
}

void Calculator::initialize() {
    binary_ops_["add"] = [](int a, int b) {
        int result = 0;
        switch (math::add(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw std::runtime_error("Overflow in addition");
        default:
            throw std::runtime_error("Unknown error in addition");
        }
    };
    binary_ops_["subtract"] = [](int a, int b) {
        int result = 0;
        switch (math::subtract(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw std::runtime_error("Overflow in subtraction");
        default:
            throw std::runtime_error("Unknown error in subtraction");
        }
    };
    binary_ops_["multiply"] = [](int a, int b) {
        int result = 0;
        switch (math::multiply(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw std::runtime_error("Overflow in multiplication");
        default:
            throw std::runtime_error("Unknown error in multiplication");
        }
    };
    binary_ops_["divide"] = [](int a, int b) {
        int result = 0;
        switch (math::divide(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::DIVISION_BY_ZERO:
            throw std::runtime_error("Division by zero");
        case math::StatusCode::OVERFLOW:
            throw std::runtime_error("Overflow in division");
        default:
            throw std::runtime_error("Unknown error in division");
        }
    };
    binary_ops_["power"] = [](int a, int b) {
        int result = 0;
        switch (math::power(a, b, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw std::runtime_error("Overflow in power");
        case math::StatusCode::INVALID_ARGUMENT:
            throw std::runtime_error("Negative exponent is not supported");
        default:
            throw std::runtime_error("Unknown error in power");
        }
    };

    unary_ops_["factorial"] = [](int a) {
        int result = 0;
        switch (math::factorial(a, result)) {
        case math::StatusCode::SUCCESS:
            return result;
        case math::StatusCode::OVERFLOW:
            throw std::runtime_error("Overflow in factorial");
        case math::StatusCode::INVALID_ARGUMENT:
            throw std::runtime_error("Factorial of negative number");
        default:
            throw std::runtime_error("Unknown error in factorial");
        }
    };
}

} // namespace app
