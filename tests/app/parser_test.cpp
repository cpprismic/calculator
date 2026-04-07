#include <gtest/gtest.h>

#include "../../src/app/parser.hpp"
#include "../../src/app/exceptions.hpp"

namespace app::test {

// Успешный парсинг

TEST(ParserTest, ParsesBinaryOperation) {
    const Task task = Parser::parse(R"({"operation": "add", "operands": [10, 5]})");

    EXPECT_EQ(task.operation, "add");
    EXPECT_EQ(task.operands, (std::vector<int>{10, 5}));
}

TEST(ParserTest, ParsesUnaryOperation) {
    const Task task = Parser::parse(R"({"operation": "factorial", "operands": [5]})");

    EXPECT_EQ(task.operation, "factorial");
    EXPECT_EQ(task.operands, (std::vector<int>{5}));
}

TEST(ParserTest, ParsesNegativeOperands) {
    const Task task = Parser::parse(R"({"operation": "add", "operands": [-10, -5]})");

    EXPECT_EQ(task.operands, (std::vector<int>{-10, -5}));
}

// Невалидный JSON

TEST(ParserTest, ThrowsOnInvalidJson) {
    EXPECT_THROW(Parser::parse("not a json"), ParseException);
}

TEST(ParserTest, ThrowsOnEmptyString) {
    EXPECT_THROW(Parser::parse(""), ParseException);
}

// Отсутствующие поля

TEST(ParserTest, ThrowsOnMissingOperation) {
    EXPECT_THROW(Parser::parse(R"({"operands": [10, 5]})"), ParseException);
}

TEST(ParserTest, ThrowsOnMissingOperands) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add"})"), ParseException);
}

// Неверные типы полей

TEST(ParserTest, ThrowsOnNonStringOperation) {
    EXPECT_THROW(Parser::parse(R"({"operation": 42, "operands": [10, 5]})"), ParseException);
}

TEST(ParserTest, ThrowsOnNonIntegerOperands) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add", "operands": [1.5, 2]})"), ParseException);
}

TEST(ParserTest, ThrowsOnNonArrayOperands) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add", "operands": 10})"), ParseException);
}

// Неверное количество операндов

TEST(ParserTest, ThrowsOnTooFewOperandsForBinary) {
    EXPECT_THROW(Parser::parse(R"({"operation": "add", "operands": [10]})"), ParseException);
}

TEST(ParserTest, ThrowsOnTooManyOperandsForUnary) {
    EXPECT_THROW(Parser::parse(R"({"operation": "factorial", "operands": [5, 3]})"), ParseException);
}

// Неизвестная операция

TEST(ParserTest, ThrowsOnUnknownOperation) {
    EXPECT_THROW(Parser::parse(R"({"operation": "unknown", "operands": [1, 2]})"), ParseException);
}

} // namespace app::test
