#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../../app/task.hpp"

namespace storage {

class Cache {
public:
    Cache() = default;

    std::optional<calculator::Task> find(const calculator::Task& task) const;
    void insert(const calculator::Task& task);

    std::size_t size() const noexcept;
    void clear() noexcept;

    static std::string makeKey(const calculator::Task& task);

private:
    std::unordered_map<std::string, calculator::Task> map_;

    static const std::unordered_set<std::string> commutative_ops_;
    static const std::unordered_set<std::string> unary_ops_;
};

} // namespace storage
