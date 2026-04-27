#pragma once

#include <libpq-fe.h>
#include <string_view>

namespace storage::db {

class PgConnection {
public:
    explicit PgConnection(std::string_view conninfo);

    ~PgConnection();

    PgConnection(const PgConnection&) = delete;
    PgConnection& operator=(const PgConnection&) = delete;

    PgConnection(PgConnection&& other) noexcept;
    PgConnection& operator=(PgConnection&& other) noexcept;

    PGconn* get() const noexcept; // NOLINT(cppcoreguidelines-owning-memory)

    bool isOk() const noexcept;

private:
    PGconn* conn_ {nullptr}; // NOLINT(cppcoreguidelines-owning-memory)

    void connect(std::string_view conninfo);
};

} // namespace storage::db
