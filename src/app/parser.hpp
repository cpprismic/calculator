#pragma once

#include <string>
#include <unordered_set>

#include "task.hpp"

namespace app {

class Parser {
public:
    // Принимает сырую JSON-строку из argv
    // Бросает ParseException если JSON невалиден или поля отсутствуют
    Task parse(const std::string& json_str) const;

private:
    void validate(const Task& task) const;

    std::unordered_set<std::string> unary_ = {"factorial"};
    std::unordered_set<std::string> binary_ = {"add", "subtract", "multiply", "divide", "power"};
};

} // namespace app