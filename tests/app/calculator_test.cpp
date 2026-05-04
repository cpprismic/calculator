#include <gtest/gtest.h>

#include "../../src/app/calculator.hpp"
#include "../../src/app/task.hpp"

namespace app::test {

calculator::Task makeTask(const std::string& op, int first, int second = 0) {
    calculator::Task task;
    task.operation = op;
    task.first_number = first;
    task.second_number = second;
    return task;
}

// Сложение

TEST(CalculatorTest, Add) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("add", 10, 5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 15);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, AddNegative) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("add", -10, -5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, -15);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, AddOverflowSetsOverflowStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("add", 2147483647, 1);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Overflow);
}

// Вычитание

TEST(CalculatorTest, Subtract) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("subtract", 10, 5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 5);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, SubtractOverflowSetsOverflowStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("subtract", -2147483648, 1);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Overflow);
}

// Умножение

TEST(CalculatorTest, Multiply) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("multiply", 6, 7);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 42);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, MultiplyByZero) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("multiply", 999, 0);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 0);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, MultiplyOverflowSetsOverflowStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("multiply", 2147483647, 2);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Overflow);
}

// Деление

TEST(CalculatorTest, Divide) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("divide", 20, 4);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 5);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, DivideRoundsTowardZero) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("divide", 7, 2);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 3);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, DivideByZeroSetsDivisionByZeroStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("divide", 10, 0);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::DivisionByZero);
}

TEST(CalculatorTest, DivideOverflowSetsOverflowStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("divide", -2147483648, -1);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Overflow);
}

// Возведение в степень

TEST(CalculatorTest, Power) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("power", 2, 10);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 1024);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, PowerZeroExponent) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("power", 999, 0);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 1);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, PowerNegativeExponentSetsFailedStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("power", 2, -3);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Failed);
}

TEST(CalculatorTest, PowerOverflowSetsOverflowStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("power", 2147483647, 2);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Overflow);
}

// Факториал

TEST(CalculatorTest, Factorial) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("factorial", 5);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 120);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, FactorialZero) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("factorial", 0);
    calculator.calculate(task);
    EXPECT_EQ(task.result, 1);
    EXPECT_EQ(task.status, calculator::OperationStatus::Success);
}

TEST(CalculatorTest, FactorialNegativeSetsFailedStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("factorial", -1);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Failed);
}

TEST(CalculatorTest, FactorialOverflowSetsOverflowStatus) {
    const calculator::Calculator calculator;
    calculator::Task task = makeTask("factorial", 20);
    calculator.calculate(task);
    EXPECT_EQ(task.status, calculator::OperationStatus::Overflow);
}

} // namespace app::test
