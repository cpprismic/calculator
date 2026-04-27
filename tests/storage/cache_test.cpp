#include <gtest/gtest.h>

#include "app/task.hpp"
#include "storage/cache/cache.hpp"

namespace {

calculator::Task makeTask(const std::string& op, int first, int second = 0,
                   int result = 0,
                   calculator::OperationStatus status = calculator::OperationStatus::NotExecuted) {
    calculator::Task task;
    task.operation = op;
    task.first_number = first;
    task.second_number = second;
    task.result = result;
    task.status = status;
    return task;
}

TEST(CacheMakeKeyTest, CommutativeAddSortsOperands) {
    const calculator::Task t1 = makeTask("add", 2, 3);
    const calculator::Task t2 = makeTask("add", 3, 2);
    EXPECT_EQ(storage::Cache::makeKey(t1), storage::Cache::makeKey(t2));
}

TEST(CacheMakeKeyTest, CommutativeMultiplySortsOperands) {
    const calculator::Task t1 = makeTask("multiply", 5, 7);
    const calculator::Task t2 = makeTask("multiply", 7, 5);
    EXPECT_EQ(storage::Cache::makeKey(t1), storage::Cache::makeKey(t2));
}

TEST(CacheMakeKeyTest, CommutativeKeyContainsSortedOperands) {
    const calculator::Task task = makeTask("add", 3, 2);
    EXPECT_EQ(storage::Cache::makeKey(task), "add:2:3");
}

TEST(CacheMakeKeyTest, NonCommutativeSubtractPreservesOrder) {
    const calculator::Task t1 = makeTask("subtract", 5, 3);
    const calculator::Task t2 = makeTask("subtract", 3, 5);
    EXPECT_NE(storage::Cache::makeKey(t1), storage::Cache::makeKey(t2));
    EXPECT_EQ(storage::Cache::makeKey(t1), "subtract:5:3");
}

TEST(CacheMakeKeyTest, NonCommutativeDividePreservesOrder) {
    const calculator::Task t1 = makeTask("divide", 10, 2);
    const calculator::Task t2 = makeTask("divide", 2, 10);
    EXPECT_NE(storage::Cache::makeKey(t1), storage::Cache::makeKey(t2));
}

TEST(CacheMakeKeyTest, NonCommutativePowerPreservesOrder) {
    const calculator::Task t1 = makeTask("power", 2, 8);
    const calculator::Task t2 = makeTask("power", 8, 2);
    EXPECT_NE(storage::Cache::makeKey(t1), storage::Cache::makeKey(t2));
}

TEST(CacheMakeKeyTest, UnaryFactorialOmitsSecondOperand) {
    const calculator::Task task = makeTask("factorial", 5, 0);
    EXPECT_EQ(storage::Cache::makeKey(task), "factorial:5");
}

TEST(CacheMakeKeyTest, UnaryFactorialIgnoresSecondOperand) {
    const calculator::Task t1 = makeTask("factorial", 5, 0);
    const calculator::Task t2 = makeTask("factorial", 5, 99);
    EXPECT_EQ(storage::Cache::makeKey(t1), storage::Cache::makeKey(t2));
}

TEST(CacheTest, FindOnEmptyCacheReturnsNullopt) {
    storage::Cache cache;
    const calculator::Task task = makeTask("add", 1, 2);
    EXPECT_FALSE(cache.find(task).has_value());
}

TEST(CacheTest, InsertThenFindReturnsTask) {
    storage::Cache cache;
    const calculator::Task task = makeTask("add", 1, 2, 3, calculator::OperationStatus::Success);
    cache.insert(task);

    const auto result = cache.find(task);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->result, 3);
    EXPECT_EQ(result->status, calculator::OperationStatus::Success);
}

TEST(CacheTest, InsertCommutativeFoundWithSwappedOperands) {
    storage::Cache cache;
    const calculator::Task inserted = makeTask("add", 2, 3, 5, calculator::OperationStatus::Success);
    cache.insert(inserted);

    const calculator::Task lookup = makeTask("add", 3, 2);
    const auto result = cache.find(lookup);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->result, 5);
}

TEST(CacheTest, InsertOverwritesExistingEntry) {
    storage::Cache cache;
    const calculator::Task first = makeTask("add", 1, 2, 3, calculator::OperationStatus::Success);
    const calculator::Task second = makeTask("add", 1, 2, 99, calculator::OperationStatus::Success);
    cache.insert(first);
    cache.insert(second);

    const auto result = cache.find(first);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->result, 99);
}

TEST(CacheTest, SizeReflectsUniqueKeys) {
    storage::Cache cache;
    cache.insert(makeTask("add", 2, 3, 5, calculator::OperationStatus::Success));
    cache.insert(makeTask("add", 3, 2, 5, calculator::OperationStatus::Success)); // same key
    cache.insert(makeTask("subtract", 5, 3, 2, calculator::OperationStatus::Success));
    EXPECT_EQ(cache.size(), 2U);
}

TEST(CacheTest, ClearResizesToZero) {
    storage::Cache cache;
    cache.insert(makeTask("add", 1, 2, 3, calculator::OperationStatus::Success));
    cache.clear();
    EXPECT_EQ(cache.size(), 0U);
}

TEST(CacheTest, FindAfterClearReturnsNullopt) {
    storage::Cache cache;
    const calculator::Task task = makeTask("add", 1, 2, 3, calculator::OperationStatus::Success);
    cache.insert(task);
    cache.clear();
    EXPECT_FALSE(cache.find(task).has_value());
}

TEST(CacheTest, FailedTaskCanBeCached) {
    storage::Cache cache;
    const calculator::Task task = makeTask("divide", 1, 0, 0, calculator::OperationStatus::Failed);
    cache.insert(task);

    const auto result = cache.find(task);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->status, calculator::OperationStatus::Failed);
}

} // namespace
