#pragma once

#include <filesystem>
#include <string>

namespace config {

struct DbConfig {
    std::string host {"localhost"};
    int port {5432};
    std::string dbname;
    std::string user;
    std::string password;

    std::string toConnString() const;
};

class Config {
public:
    explicit Config(const std::filesystem::path& path);

    const DbConfig& db() const noexcept;

private:
    DbConfig db_;
};

} // namespace config
