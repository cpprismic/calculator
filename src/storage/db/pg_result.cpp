#include "pg_result.hpp"

#include <stdexcept>
#include <string>

namespace storage::db {

PgResult::PgResult(PGresult* result) noexcept : result_ {result} {}

PGresult* PgResult::get() const noexcept {
    return result_.get();
}

ExecStatusType PgResult::status() const noexcept {
    return PQresultStatus(result_.get());
}

int PgResult::numRows() const noexcept {
    return PQntuples(result_.get());
}

std::string PgResult::getValue(int row, int col) const {
    if (row < 0 || row >= numRows() || col < 0 || col >= PQnfields(result_.get())) {
        throw std::out_of_range("PgResult::getValue: invalid row/col (" + std::to_string(row) +
                                ", " + std::to_string(col) + ")");
    }
    return std::string {PQgetvalue(result_.get(), row, col)};
}

} // namespace storage::db
