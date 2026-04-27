#pragma once

#include <libpq-fe.h>
#include <string>

namespace storage::db {

class PgResult {
public:
    explicit PgResult(PGresult* result) noexcept; // NOLINT(cppcoreguidelines-owning-memory)

    ~PgResult();

    PgResult(const PgResult&) = delete;
    PgResult& operator=(const PgResult&) = delete;

    PgResult(PgResult&& other) noexcept;
    PgResult& operator=(PgResult&& other) noexcept;

    PGresult* get() const noexcept; // NOLINT(cppcoreguidelines-owning-memory)

    ExecStatusType status() const noexcept;
    int numRows() const noexcept;

    std::string getValue(int row, int col) const;

private:
    PGresult* result_ {nullptr}; // NOLINT(cppcoreguidelines-owning-memory)
};

} // namespace storage::db
