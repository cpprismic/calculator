#include <iostream>

#include "app/calculator.hpp"
#include "app/exceptions.hpp"
#include "app/parser.hpp"
#include "logger/logger.hpp"

int main(int argc, char** argv) {
    auto& log = logger::Logger::getInstance();

    if (argc != 2) {
        log.error("Usage: calculator '<json>'");
        return 1;
    }

    try {
        const app::Parser parser;
        const app::Calculator calculator;

        const auto task = parser.parse(argv[1]);
        const int result = calculator.calculate(task);

        std::cout << result << '\n';

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