#include <gtest/gtest.h>

#include <stdexcept>

#include "../../src/app/parser.hpp"

namespace app::test {

// Успешный парсинг

TEST(ParserTest, ParsesBinaryOperation) {
    const calculator::Task task = Parser::parse(R"({"operation": "add", "operands": [10, 5]})");

    EXPECT_EQ(task.operation, "add");
    EXPECT_EQ(task.first_number, 10);
    EXPECT_EQ(task.second_number, 5);
}

TEST(ParserTest, ParsesUnaryOperation) {
    const calculator::Task task = Parser::parse(R"({"operation": "factorial", "operands": [5]})");

    EXPECT_EQ(task.operation, "factorial");
    EXPECT_EQ(task.first_number, 5);
}

TEST(ParserTest, ParsesNegativeOperands) {
    const calculator::Task task = Parser::parse(R"({"operation": "add", "operands": [-10, -5]})");

    EXPECT_EQ(task.first_number, -10);
    EXPECT_EQ(task.second_number, -5);
}

// Невалидный JSON

TEST(ParserTest, ThrowsOnInvalidJson) {
    EXPECT_THROW(Parser::parse("not a json"), std::invalid_argument);
}

TEST(ParserTest, ThrowsOnEmptyString) {
    EXPECT_THROW(Parser::parse(""), std::invalid_argument);
}

// Отсутствующие поля

TEST(ParserTest, ThrowsOnMissingOperation) {
    EXPECT_THROW(Parser::parse(R"({"operands": [10, 5]})"), std::invalid_argument);
}

TEST(ParserTest, ThrowsOnMissingOperands) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add"})"), std::invalid_argument);
}

// Неверные типы полей

TEST(ParserTest, ThrowsOnNonStringOperation) {
    EXPECT_THROW(Parser::parse(R"({"operation": 42, "operands": [10, 5]})"), std::invalid_argument);
}

TEST(ParserTest, ThrowsOnNonIntegerOperands) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add", "operands": [1.5, 2]})"),
                 std::invalid_argument);
}

TEST(ParserTest, ThrowsOnNonArrayOperands) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add", "operands": 10})"), std::invalid_argument);
}

// Неверное количество операндов

TEST(ParserTest, ThrowsOnTooFewOperandsForBinary) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add", "operands": [10]})"), std::invalid_argument);
}

TEST(ParserTest, ThrowsOnTooManyOperandsForUnary) {
    EXPECT_THROW(Parser::parse(R"({"operation": "factorial", "operands": [5, 3]})"),
                 std::invalid_argument);
}

// Неизвестная операция

TEST(ParserTest, ThrowsOnUnknownOperation) {
    EXPECT_THROW(Parser::parse(R"({"operation": "unknown", "operands": [1, 2]})"),
                 std::invalid_argument);
}

} // namespace app::test
