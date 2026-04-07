#include <gtest/gtest.h>

#include "../../src/app/parser.hpp"
#include "../../src/app/exceptions.hpp"

namespace app::test {

// Успешный парсинг

TEST(ParserTest, ParsesBinaryOperation) {
    const Parser parser;
    const Task task = parser.parse(R"({"operation": "add", "operands": [10, 5]})");

    EXPECT_EQ(task.operation, "add");
    EXPECT_EQ(task.operands, (std::vector<int>{10, 5}));
}

TEST(ParserTest, ParsesUnaryOperation) {
    const Parser parser;
    const Task task = parser.parse(R"({"operation": "factorial", "operands": [5]})");

    EXPECT_EQ(task.operation, "factorial");
    EXPECT_EQ(task.operands, (std::vector<int>{5}));
}

TEST(ParserTest, ParsesNegativeOperands) {
    const Parser parser;
    const Task task = parser.parse(R"({"operation": "add", "operands": [-10, -5]})");

    EXPECT_EQ(task.operands, (std::vector<int>{-10, -5}));
}

// Невалидный JSON

TEST(ParserTest, ThrowsOnInvalidJson) {
    const Parser parser;
    EXPECT_THROW(parser.parse("not a json"), ParseException);
}

TEST(ParserTest, ThrowsOnEmptyString) {
    const Parser parser;
    EXPECT_THROW(parser.parse(""), ParseException);
}

// Отсутствующие поля

TEST(ParserTest, ThrowsOnMissingOperation) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operands": [10, 5]})"), ParseException);
}

TEST(ParserTest, ThrowsOnMissingOperands) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operation": "add"})"), ParseException);
}

// Неверные типы полей

TEST(ParserTest, ThrowsOnNonStringOperation) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operation": 42, "operands": [10, 5]})"), ParseException);
}

TEST(ParserTest, ThrowsOnNonIntegerOperands) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operation": "add", "operands": [1.5, 2]})"), ParseException);
}

TEST(ParserTest, ThrowsOnNonArrayOperands) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operation": "add", "operands": 10})"), ParseException);
}

// Неверное количество операндов

TEST(ParserTest, ThrowsOnTooFewOperandsForBinary) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operation": "add", "operands": [10]})"), ParseException);
}

TEST(ParserTest, ThrowsOnTooManyOperandsForUnary) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operation": "factorial", "operands": [5, 3]})"), ParseException);
}

// Неизвестная операция

TEST(ParserTest, ThrowsOnUnknownOperation) {
    const Parser parser;
    EXPECT_THROW(parser.parse(R"({"operation": "unknown", "operands": [1, 2]})"), ParseException);
}

} // namespace app::test