#include "pg_result.hpp"

#include <stdexcept>
#include <string>

namespace storage::db {

PgResult::PgResult(PGresult* result) noexcept // NOLINT(cppcoreguidelines-owning-memory)
    : result_(result) {}

PgResult::~PgResult() {
    if (result_ != nullptr) {
        PQclear(result_); // NOLINT(cppcoreguidelines-owning-memory)
        result_ = nullptr;
    }
}

PgResult::PgResult(PgResult&& other) noexcept : result_(other.result_) {
    other.result_ = nullptr;
}

PgResult& PgResult::operator=(PgResult&& other) noexcept {
    if (this != &other) {
        if (result_ != nullptr) {
            PQclear(result_); // NOLINT(cppcoreguidelines-owning-memory)
        }
        result_ = other.result_;
        other.result_ = nullptr;
    }
    return *this;
}

PGresult* PgResult::get() const noexcept {
    return result_;
}

ExecStatusType PgResult::status() const noexcept {
    return PQresultStatus(result_);
}

int PgResult::numRows() const noexcept {
    return PQntuples(result_);
}

std::string PgResult::getValue(int row, int col) const {
    if (row < 0 || row >= numRows() || col < 0 || col >= PQnfields(result_)) {
        throw std::out_of_range("PgResult::getValue: invalid row/col (" + std::to_string(row) +
                                ", " + std::to_string(col) + ")");
    }
    return std::string {PQgetvalue(result_, row, col)};
}

} // namespace storage::db
