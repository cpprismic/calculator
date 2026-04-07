#pragma once

#include <string>
#include <unordered_set>

#include "task.hpp"

namespace app {

class Parser {
public:
    // Принимает сырую JSON-строку из argv
    // Бросает ParseException если JSON невалиден или поля отсутствуют
    static Task parse(const std::string& json_str);

private:
    static void validate(const Task& task);

    static const std::unordered_set<std::string> unary_;
    static const std::unordered_set<std::string> binary_;
};

} // namespace app