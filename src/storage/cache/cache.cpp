#include "cache.hpp"

#include <algorithm>
#include <string>

namespace storage {

const std::unordered_set<std::string> Cache::commutative_ops_ = {"add", "multiply"};
const std::unordered_set<std::string> Cache::unary_ops_ = {"factorial"};

std::string Cache::makeKey(const calculator::Task& task) {
    if (unary_ops_.count(task.operation) != 0) {
        return task.operation + ":" + std::to_string(task.first_number);
    }

    if (commutative_ops_.count(task.operation) != 0) {
        const int lo = std::min(task.first_number, task.second_number);
        const int hi = std::max(task.first_number, task.second_number);
        return task.operation + ":" + std::to_string(lo) + ":" + std::to_string(hi);
    }

    return task.operation + ":" + std::to_string(task.first_number) + ":" +
           std::to_string(task.second_number);
}

std::optional<calculator::Task> Cache::find(const calculator::Task& task) const {
    const auto it = map_.find(makeKey(task));
    if (it == map_.end()) {
        return std::nullopt;
    }
    return it->second;
}

void Cache::insert(const calculator::Task& task) {
    map_.insert_or_assign(makeKey(task), task);
}

std::size_t Cache::size() const noexcept {
    return map_.size();
}

void Cache::clear() noexcept {
    map_.clear();
}

} // namespace storage
