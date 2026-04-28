#include "calculator.hpp"

#include "../logger/logger.hpp"
#include "math.hpp"

namespace {

calculator::OperationStatus statusFromCode(math::StatusCode code) {
    switch (code) {
    case math::StatusCode::SUCCESS:
        return calculator::OperationStatus::Success;
    case math::StatusCode::DIVISION_BY_ZERO:
        return calculator::OperationStatus::DivisionByZero;
    case math::StatusCode::OVERFLOW:
        return calculator::OperationStatus::Overflow;
    default:
        return calculator::OperationStatus::Failed;
    }
}

} // namespace

namespace calculator {

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

    int result = 0;
    if (const auto it = binary_ops_.find(task.operation); it != binary_ops_.end()) {
        log.debug("Second number: " + std::to_string(task.second_number));
        task.status = statusFromCode(it->second(task.first_number, task.second_number, result));
    } else if (const auto it = unary_ops_.find(task.operation); it != unary_ops_.end()) {
        task.status = statusFromCode(it->second(task.first_number, result));
    } else {
        task.status = OperationStatus::Failed;
    }

    if (task.status == OperationStatus::Success) {
        task.result = result;
    }

    log.debug("Result: " + std::to_string(task.result));
}

void Calculator::initialize() {
    binary_ops_["add"] = math::add;
    binary_ops_["subtract"] = math::subtract;
    binary_ops_["multiply"] = math::multiply;
    binary_ops_["divide"] = math::divide;
    binary_ops_["power"] = math::power;
    unary_ops_["factorial"] = math::factorial;
}

} // namespace calculator
