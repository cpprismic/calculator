#pragma once

#include <optional>

#include "../app/task.hpp"
#include "cache/cache.hpp"
#include "db/repository.hpp"

namespace storage {

// Lifecycle:
//   Construction: connects to the DB and warms up the cache from all stored records.
//   If the DB is unavailable the Storage degrades gracefully: the cache still works
//   for the process lifetime, but results are not persisted.
//
//   find(): cache lookup only — always O(1).
//   save(): inserts into the cache; also writes to the DB when it is available.
class Storage {
public:
    Storage();

    ~Storage() = default;

    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;

    Storage(Storage&&) noexcept = default;
    Storage& operator=(Storage&&) noexcept = default;

    std::optional<calculator::Task> find(const calculator::Task& task) const;

    void save(const calculator::Task& task);

    bool isDbAvailable() const noexcept;

private:
    Cache cache_;
    std::optional<db::Repository> repo_;

    void warmUp();
};

} // namespace storage
