#include "application.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>

#include "../logger/logger.hpp"
#include "parser.hpp"

namespace app {

Application::Application() = default;
Application::~Application() = default;

Application::Application(const Application&) = default;
Application& Application::operator=(const Application&) = default;

Application::Application(Application&&) noexcept = default;
Application& Application::operator=(Application&&) noexcept = default;

void Application::run(int argc, char** argv) {
    auto& log = logger::Logger::getInstance();

    if (argc != 2) {
        log.error("Usage: calculator '<json>'");
        throw std::invalid_argument("Usage: calculator '<json>'");
    }

    log.debug("Input JSON: " + std::string(argv[1]));

    Task task = Parser::parse(argv[1]);
    log.info("Operation: " + task.operation);

    calculator_.calculate(task);

    log.info("Result: " + std::to_string(task.result));
    printf("Result: %i\n", task.result);
}

} // namespace app
