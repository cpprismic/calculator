#pragma once

#include <libpq-fe.h>
#include <memory>
#include <string>

namespace storage::db {

struct PqResultDeleter {
    void operator()(PGresult* result) const noexcept { PQclear(result); }
};

class PgResult {
public:
    explicit PgResult(PGresult* result) noexcept;

    ~PgResult() = default;

    PgResult(const PgResult&) = delete;
    PgResult& operator=(const PgResult&) = delete;

    PgResult(PgResult&&) noexcept = default;
    PgResult& operator=(PgResult&&) noexcept = default;

    PGresult* get() const noexcept;

    ExecStatusType status() const noexcept;
    int numRows() const noexcept;

    std::string getValue(int row, int col) const;

private:
    std::unique_ptr<PGresult, PqResultDeleter> result_;
};

} // namespace storage::db
