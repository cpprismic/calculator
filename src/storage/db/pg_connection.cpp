#include "pg_connection.hpp"

#include <stdexcept>
#include <string>

namespace storage::db {

PgConnection::PgConnection(std::string_view conninfo) {
    connect(conninfo);
}

PGconn* PgConnection::get() const noexcept {
    return conn_.get();
}

bool PgConnection::isOk() const noexcept {
    return conn_ && PQstatus(conn_.get()) == CONNECTION_OK;
}

void PgConnection::connect(std::string_view conninfo) {
    const std::string conninfo_str {conninfo};
    conn_.reset(PQconnectdb(conninfo_str.c_str()));
    if (!isOk()) {
        std::string error_msg {"Failed to connect to PostgreSQL"};
        if (conn_) {
            error_msg += ": ";
            error_msg += PQerrorMessage(conn_.get());
            conn_.reset();
        }
        throw std::runtime_error(error_msg);
    }
}

} // namespace storage::db
