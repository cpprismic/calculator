#include "application.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>

#include "../logger/logger.hpp"
#include "parser.hpp"

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
        if (task.status == calculator::OperationStatus::Failed) {
            throw std::runtime_error("Cached failure for operation: " + task.operation);
        }
    } else {
        log.debug("Cache miss, computing...");
        try {
            calculator_.calculate(task);
        } catch (const std::runtime_error&) {
            task.status = calculator::OperationStatus::Failed;
            storage_.save(task);
            throw;
        }
        storage_.save(task);
    }

    log.info("Result: " + std::to_string(task.result));
    printf("Result: %i\n", task.result);
}

} // namespace app
