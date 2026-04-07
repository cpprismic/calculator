#include "app/calculator.hpp"
#include "app/exceptions.hpp"
#include "app/parser.hpp"
#include "logger/logger.hpp"

int main(int argc, char** argv) {
    auto& log = logger::Logger::getInstance();

#ifdef APP_DEBUG
    log.setLevel(logger::LogLevel::Debug);
    log.debug("Build type: Debug -- log level set to DEBUG");
#else
    log.setLevel(logger::LogLevel::Info);
#endif

    if (argc != 2) {
        log.error("Usage: calculator '<json>'");
        return 1;
    }

    log.debug("Input JSON: " + std::string(argv[1]));

    try {
        const app::Calculator calculator;

        const auto task = app::Parser::parse(argv[1]);
        log.info("Operation: " + task.operation);
        const int result = calculator.calculate(task);

        log.info("Result: " + std::to_string(result));
        printf("Result: %i\n", result);

    } catch (const app::ParseException& e) {
        log.error("Parse error: " + std::string(e.what()));
        return 1;
    } catch (const app::CalculationException& e) {
        log.error("Calculation error: " + std::string(e.what()));
        return 1;
    } catch (const std::exception& e) {
        log.error("Unexpected error: " + std::string(e.what()));
        return 1;
    }

    return 0;
}