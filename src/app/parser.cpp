#include "parser.hpp"

#include <stdexcept>

#include <nlohmann/json.hpp>

#include "../logger/logger.hpp"

namespace app {

const std::unordered_set<std::string> Parser::unary_ = {"factorial"};
const std::unordered_set<std::string> Parser::binary_ = {
    "add", "subtract", "multiply", "divide", "power"};

calculator::Task Parser::parse(const std::string& json_str) {
    auto& log = logger::Logger::getInstance();
    log.debug("Parsing JSON: " + json_str);

    nlohmann::json json;
    try {
        json = nlohmann::json::parse(json_str);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::invalid_argument("Invalid JSON: " + std::string(e.what()));
    }

    if (!json.contains("operation") || !json["operation"].is_string()) {
        throw std::invalid_argument("Missing or invalid field: 'operation'");
    }
    if (!json.contains("operands") || !json["operands"].is_array()) {
        throw std::invalid_argument("Missing or invalid field: 'operands'");
    }

    calculator::Task task;
    task.operation = json["operation"].get<std::string>();

    const auto& operands = json["operands"];
    for (const auto& operand : operands) {
        if (!operand.is_number_integer()) {
            throw std::invalid_argument("Operands must be integers");
        }
    }

    validate(task.operation, operands.size());

    if (binary_.count(task.operation) != 0) {
        task.first_number = operands[0].get<int>();
        task.second_number = operands[1].get<int>();
        log.debug("First number: " + std::to_string(task.first_number));
        log.debug("Second number: " + std::to_string(task.second_number));
    } else {
        task.first_number = operands[0].get<int>();
        log.debug("First number: " + std::to_string(task.first_number));
    }

    log.debug("Parsed task: operation=" + task.operation);
    return task;
}

void Parser::validate(const std::string& operation, std::size_t operand_count) {
    if (binary_.count(operation) != 0) {
        if (operand_count != 2) {
            throw std::invalid_argument("Operation '" + operation +
                                        "' requires exactly 2 operands");
        }
    } else if (unary_.count(operation) != 0) {
        if (operand_count != 1) {
            throw std::invalid_argument("Operation '" + operation + "' requires exactly 1 operand");
        }
    } else {
        throw std::invalid_argument("Unknown operation: '" + operation + "'");
    }
}

} // namespace app
