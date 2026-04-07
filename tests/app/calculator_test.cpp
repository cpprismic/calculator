#include <gtest/gtest.h>

#include "../../src/app/calculator.hpp"
#include "../../src/app/exceptions.hpp"
#include "../../src/app/task.hpp"

namespace app::test {

// Вспомогательная функция собирает Task
Task makeTask(const std::string& op, std::vector<int> operands) {
    Task task;
    task.operation = op;
    task.operands  = std::move(operands);
    return task;
}

// Сложение

TEST(CalculatorTest, Add) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("add", {10, 5})), 15);
}

TEST(CalculatorTest, AddNegative) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("add", {-10, -5})), -15);
}

TEST(CalculatorTest, AddOverflowThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("add", {2147483647, 1})), CalculationException);
}

// Вычитание

TEST(CalculatorTest, Subtract) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("subtract", {10, 5})), 5);
}

TEST(CalculatorTest, SubtractOverflowThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("subtract", {-2147483648, 1})), CalculationException);
}

// Умножение

TEST(CalculatorTest, Multiply) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("multiply", {6, 7})), 42);
}

TEST(CalculatorTest, MultiplyByZero) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("multiply", {999, 0})), 0);
}

TEST(CalculatorTest, MultiplyOverflowThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("multiply", {2147483647, 2})), CalculationException);
}

// Деление

TEST(CalculatorTest, Divide) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("divide", {20, 4})), 5);
}

TEST(CalculatorTest, DivideRoundsTowardZero) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("divide", {7, 2})), 3);
}

TEST(CalculatorTest, DivideByZeroThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("divide", {10, 0})), CalculationException);
}

// Возведение в степень

TEST(CalculatorTest, Power) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("power", {2, 10})), 1024);
}

TEST(CalculatorTest, PowerZeroExponent) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("power", {999, 0})), 1);
}

TEST(CalculatorTest, PowerNegativeExponentThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("power", {2, -3})), CalculationException);
}

TEST(CalculatorTest, PowerOverflowThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("power", {2147483647, 2})), CalculationException);
}

// Факториал

TEST(CalculatorTest, Factorial) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("factorial", {5})), 120);
}

TEST(CalculatorTest, FactorialZero) {
    const Calculator calculator;
    EXPECT_EQ(calculator.calculate(makeTask("factorial", {0})), 1);
}

TEST(CalculatorTest, FactorialNegativeThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("factorial", {-1})), CalculationException);
}

TEST(CalculatorTest, FactorialOverflowThrows) {
    const Calculator calculator;
    EXPECT_THROW(calculator.calculate(makeTask("factorial", {20})), CalculationException);
}

} // namespace app::test