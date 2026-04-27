#pragma once

#include <cstddef>
#include <string>
#include <unordered_set>

#include "task.hpp"

namespace app {

class Parser {
public:
    Parser() = delete;
    ~Parser() = delete;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    Parser(Parser&&) = delete;
    Parser& operator=(Parser&&) = delete;

    static calculator::Task parse(const std::string& json_str);

private:
    static void validate(const std::string& operation, std::size_t operand_count);

    static const std::unordered_set<std::string> unary_;
    static const std::unordered_set<std::string> binary_;
};

} // namespace app
