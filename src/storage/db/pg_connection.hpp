#pragma once

#include <libpq-fe.h>
#include <memory>
#include <string_view>

namespace storage::db {

struct PqConnDeleter {
    void operator()(PGconn* conn) const noexcept { PQfinish(conn); }
};

class PgConnection {
public:
    explicit PgConnection(std::string_view conninfo);

    ~PgConnection() = default;

    PgConnection(const PgConnection&) = delete;
    PgConnection& operator=(const PgConnection&) = delete;

    PgConnection(PgConnection&&) noexcept = default;
    PgConnection& operator=(PgConnection&&) noexcept = default;

    PGconn* get() const noexcept;

    bool isOk() const noexcept;

private:
    std::unique_ptr<PGconn, PqConnDeleter> conn_;

    void connect(std::string_view conninfo);
};

} // namespace storage::db
