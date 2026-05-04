#pragma once

#include <string_view>
#include <vector>

#include "../../app/task.hpp"
#include "pg_connection.hpp"
#include "pg_result.hpp"

namespace storage::db {

class Repository {
public:
    explicit Repository(std::string_view conninfo);

    ~Repository() = default;

    Repository(const Repository&) = delete;
    Repository& operator=(const Repository&) = delete;

    Repository(Repository&&) noexcept = default;
    Repository& operator=(Repository&&) noexcept = default;

    void save(const calculator::Task& task);

    std::vector<calculator::Task> loadAll();

private:
    PgConnection conn_;

    void ensureSchema();
    PgResult exec(std::string_view sql);
    PgResult execParams(std::string_view sql, const std::vector<const char*>& params);
};

} // namespace storage::db
