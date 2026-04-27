#include "config.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

namespace config {

namespace {

std::string quoteConnValue(const std::string& value) {
    std::string result {"'"};
    for (const char c : value) {
        if (c == '\'' || c == '\\') {
            result += '\\';
        }
        result += c;
    }
    result += '\'';
    return result;
}

} // namespace

std::string DbConfig::toConnString() const {
    return "host=" + quoteConnValue(host) + " port=" + std::to_string(port) +
           " dbname=" + quoteConnValue(dbname) + " user=" + quoteConnValue(user) +
           " password=" + quoteConnValue(password);
}

Config::Config(const std::filesystem::path& path) {
    std::ifstream file {path};
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + path.string());
    }

    try {
        const auto json = nlohmann::json::parse(file);
        const auto& db = json.at("database");

        db_.host = db.at("host").get<std::string>();
        db_.port = db.at("port").get<int>();
        db_.dbname = db.at("dbname").get<std::string>();
        db_.user = db.at("user").get<std::string>();
        db_.password = db.at("password").get<std::string>();
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("Invalid config file '" + path.string() + "': " + e.what());
    }
}

const DbConfig& Config::db() const noexcept {
    return db_;
}

} // namespace config
