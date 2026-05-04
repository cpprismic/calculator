#include "application.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>

#include "../logger/logger.hpp"
#include "parser.hpp"

namespace {

std::string errorMessage(const calculator::Task& task) {
    switch (task.status) {
    case calculator::OperationStatus::DivisionByZero:
        return "Division by zero: " + task.operation;
    case calculator::OperationStatus::Overflow:
        return "Overflow: " + task.operation;
    default:
        return "Calculation failed: " + task.operation;
    }
}

} // namespace

namespace app {

Application::Application() = default;
Application::~Application() = default;

void Application::run(int argc, char** argv) {
    auto& log = logger::Logger::getInstance();

    if (argc != 2) {
        log.error("Usage: calculator '<json>'");
        throw std::invalid_argument("Usage: calculator '<json>'");
    }

    log.debug("Input JSON: " + std::string(argv[1]));

    calculator::Task task = Parser::parse(argv[1]);
    log.info("Operation: " + task.operation);

    if (auto cached = storage_.find(task)) {
        log.info("Cache hit for operation: " + task.operation);
        task = *cached;
        if (task.status != calculator::OperationStatus::Success) {
            throw std::runtime_error(errorMessage(task));
        }
    } else {
        log.debug("Cache miss, computing...");
        calculator_.calculate(task);
        storage_.save(task);
        if (task.status != calculator::OperationStatus::Success) {
            throw std::runtime_error(errorMessage(task));
        }
    }

    log.info("Result: " + std::to_string(task.result));
    printf("Result: %i\n", task.result);
}

} // namespace app
