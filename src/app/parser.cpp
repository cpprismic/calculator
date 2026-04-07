#include "parser.hpp"

#include <nlohmann/json.hpp>

#include "../logger/logger.hpp"
#include "exceptions.hpp"

namespace app {

const std::unordered_set<std::string> Parser::unary_ = {"factorial"};
const std::unordered_set<std::string> Parser::binary_ = {
    "add", "subtract", "multiply", "divide", "power"};

Task Parser::parse(const std::string& json_str) {
    auto& log = logger::Logger::getInstance();
    log.debug("Parsing JSON: " + json_str);

    nlohmann::json json;
    try {
        json = nlohmann::json::parse(json_str);
    } catch (const nlohmann::json::parse_error& e) {
        throw ParseException("Invalid JSON: " + std::string(e.what()));
    }

    if (!json.contains("operation") || !json["operation"].is_string()) {
        throw ParseException("Missing or invalid field: 'operation'");
    }
    if (!json.contains("operands") || !json["operands"].is_array()) {
        throw ParseException("Missing or invalid field: 'operands'");
    }

    Task task;
    task.operation = json["operation"].get<std::string>();

    for (const auto& operand : json["operands"]) {
        if (!operand.is_number_integer()) {
            throw ParseException("Operands must be integers");
        }
        task.operands.push_back(operand.get<int>());
    }

    validate(task);

    log.debug("Parsed task: operation=" + task.operation);
    return task;
}

void Parser::validate(const Task& task) {
    if (binary_.count(task.operation) != 0) {
        if (task.operands.size() != 2) {
            throw ParseException("Operation '" + task.operation + "' requires exactly 2 operands");
        }
    } else if (unary_.count(task.operation) != 0) {
        if (task.operands.size() != 1) {
            throw ParseException("Operation '" + task.operation + "' requires exactly 1 operand");
        }
    } else {
        throw ParseException("Unknown operation: '" + task.operation + "'");
    }
}

} // namespace app