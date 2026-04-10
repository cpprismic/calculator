#include <gtest/gtest.h>

#include <stdexcept>

#include "../../src/app/calculator.hpp"
#include "../../src/app/task.hpp"

namespace app::test {

Task makeTask(const std::string& op, int first, int second = 0) {
    Task task;
    task.operation = op;
    task.first_number = first;
    task.second_number = second;
    return task;
}

// Сложение

TEST(CalculatorTest, Add) {
    const Calculator calculator;
    Task task = makeTask("add", 10, 5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 15);
}

TEST(CalculatorTest, AddNegative) {
    const Calculator calculator;
    Task task = makeTask("add", -10, -5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, -15);
}

TEST(CalculatorTest, AddOverflowThrows) {
    const Calculator calculator;
    Task task = makeTask("add", 2147483647, 1);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

// Вычитание

TEST(CalculatorTest, Subtract) {
    const Calculator calculator;
    Task task = makeTask("subtract", 10, 5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 5);
}

TEST(CalculatorTest, SubtractOverflowThrows) {
    const Calculator calculator;
    Task task = makeTask("subtract", -2147483648, 1);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

// Умножение

TEST(CalculatorTest, Multiply) {
    const Calculator calculator;
    Task task = makeTask("multiply", 6, 7);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 42);
}

TEST(CalculatorTest, MultiplyByZero) {
    const Calculator calculator;
    Task task = makeTask("multiply", 999, 0);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 0);
}

TEST(CalculatorTest, MultiplyOverflowThrows) {
    const Calculator calculator;
    Task task = makeTask("multiply", 2147483647, 2);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

// Деление

TEST(CalculatorTest, Divide) {
    const Calculator calculator;
    Task task = makeTask("divide", 20, 4);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 5);
}

TEST(CalculatorTest, DivideRoundsTowardZero) {
    const Calculator calculator;
    Task task = makeTask("divide", 7, 2);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 3);
}

TEST(CalculatorTest, DivideByZeroThrows) {
    const Calculator calculator;
    Task task = makeTask("divide", 10, 0);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

TEST(CalculatorTest, DivideOverflowThrows) {
    const Calculator calculator;
    Task task = makeTask("divide", -2147483648, -1);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

// Возведение в степень

TEST(CalculatorTest, Power) {
    const Calculator calculator;
    Task task = makeTask("power", 2, 10);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 1024);
}

TEST(CalculatorTest, PowerZeroExponent) {
    const Calculator calculator;
    Task task = makeTask("power", 999, 0);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 1);
}

TEST(CalculatorTest, PowerNegativeExponentThrows) {
    const Calculator calculator;
    Task task = makeTask("power", 2, -3);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

TEST(CalculatorTest, PowerOverflowThrows) {
    const Calculator calculator;
    Task task = makeTask("power", 2147483647, 2);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

// Факториал

TEST(CalculatorTest, Factorial) {
    const Calculator calculator;
    Task task = makeTask("factorial", 5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 120);
}

TEST(CalculatorTest, FactorialZero) {
    const Calculator calculator;
    Task task = makeTask("factorial", 0);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 1);
}

TEST(CalculatorTest, FactorialNegativeThrows) {
    const Calculator calculator;
    Task task = makeTask("factorial", -1);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

TEST(CalculatorTest, FactorialOverflowThrows) {
    const Calculator calculator;
    Task task = makeTask("factorial", 20);
    EXPECT_THROW(calculator.calculate(task), std::runtime_error);
}

} // namespace app::test
