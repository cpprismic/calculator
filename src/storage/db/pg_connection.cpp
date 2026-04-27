#include "pg_connection.hpp"

#include <stdexcept>
#include <string>

namespace storage::db {

PgConnection::PgConnection(std::string_view conninfo) {
    connect(conninfo);
}

PgConnection::~PgConnection() {
    if (conn_ != nullptr) {
        PQfinish(conn_); // NOLINT(cppcoreguidelines-owning-memory)
        conn_ = nullptr;
    }
}

PgConnection::PgConnection(PgConnection&& other) noexcept : conn_(other.conn_) {
    other.conn_ = nullptr;
}

PgConnection& PgConnection::operator=(PgConnection&& other) noexcept {
    if (this != &other) {
        if (conn_ != nullptr) {
            PQfinish(conn_); // NOLINT(cppcoreguidelines-owning-memory)
        }
        conn_ = other.conn_;
        other.conn_ = nullptr;
    }
    return *this;
}

PGconn* PgConnection::get() const noexcept {
    return conn_;
}

bool PgConnection::isOk() const noexcept {
    return conn_ != nullptr && PQstatus(conn_) == CONNECTION_OK;
}

void PgConnection::connect(std::string_view conninfo) {
    const std::string conninfo_str {conninfo};
    conn_ = PQconnectdb(conninfo_str.c_str()); // NOLINT(cppcoreguidelines-owning-memory)
    if (!isOk()) {
        std::string error_msg {"Failed to connect to PostgreSQL"};
        if (conn_ != nullptr) {
            error_msg += ": ";
            error_msg += PQerrorMessage(conn_);
            PQfinish(conn_); // NOLINT(cppcoreguidelines-owning-memory)
            conn_ = nullptr;
        }
        throw std::runtime_error(error_msg);
    }
}

} // namespace storage::db
