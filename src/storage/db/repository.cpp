#include "repository.hpp"

#include <stdexcept>
#include <string>

#include "../../logger/logger.hpp"

namespace storage::db {

namespace {

calculator::OperationStatus statusFromInt(int value) {
    switch (value) {
    case 0:
        return calculator::OperationStatus::Success;
    case 2:
        return calculator::OperationStatus::DivisionByZero;
    case 3:
        return calculator::OperationStatus::Overflow;
    default:
        return calculator::OperationStatus::Failed;
    }
}

int statusToInt(calculator::OperationStatus status) {
    switch (status) {
    case calculator::OperationStatus::Success:
        return 0;
    case calculator::OperationStatus::DivisionByZero:
        return 2;
    case calculator::OperationStatus::Overflow:
        return 3;
    default:
        return 1;
    }
}

} // namespace

Repository::Repository(std::string_view conninfo) : conn_ {conninfo} {
    ensureSchema();
}

void Repository::ensureSchema() {
    exec("SET client_min_messages = WARNING");
    PgResult result = exec("CREATE TABLE IF NOT EXISTS calculations ("
                           "    id         SERIAL PRIMARY KEY,"
                           "    operation  VARCHAR(32) NOT NULL,"
                           "    first_num  INTEGER     NOT NULL,"
                           "    second_num INTEGER     NOT NULL,"
                           "    result     INTEGER     NOT NULL,"
                           "    status     INTEGER     NOT NULL"
                           ")");
    if (result.status() != PGRES_COMMAND_OK) {
        throw std::runtime_error(std::string {"Failed to create schema: "} +
                                 PQerrorMessage(conn_.get()));
    }
}

void Repository::save(const calculator::Task& task) {
    const std::string op_str = task.operation;
    const std::string first_str = std::to_string(task.first_number);
    const std::string second_str = std::to_string(task.second_number);
    const std::string result_str = std::to_string(task.result);
    const std::string status_str = std::to_string(statusToInt(task.status));

    const std::vector<const char*> params = {op_str.c_str(),
                                             first_str.c_str(),
                                             second_str.c_str(),
                                             result_str.c_str(),
                                             status_str.c_str()};

    PgResult res =
        execParams("INSERT INTO calculations (operation, first_num, second_num, result, status) "
                   "VALUES ($1, $2, $3, $4, $5)",
                   params);

    if (res.status() != PGRES_COMMAND_OK) {
        throw std::runtime_error(std::string {"Failed to save task to DB: "} +
                                 PQerrorMessage(conn_.get()));
    }
}

std::vector<calculator::Task> Repository::loadAll() {
    PgResult result =
        exec("SELECT operation, first_num, second_num, result, status FROM calculations");

    if (result.status() != PGRES_TUPLES_OK) {
        throw std::runtime_error(std::string {"Failed to load tasks from DB: "} +
                                 PQerrorMessage(conn_.get()));
    }

    std::vector<calculator::Task> tasks;
    tasks.reserve(static_cast<std::size_t>(result.numRows()));

    for (int i = 0; i < result.numRows(); ++i) {
        try {
            calculator::Task task;
            task.operation = result.getValue(i, 0);
            task.first_number = std::stoi(result.getValue(i, 1));
            task.second_number = std::stoi(result.getValue(i, 2));
            task.result = std::stoi(result.getValue(i, 3));
            task.status = statusFromInt(std::stoi(result.getValue(i, 4)));
            tasks.push_back(std::move(task));
        } catch (const std::invalid_argument& e) {
            logger::Logger::getInstance().warning("Skipping malformed DB row " + std::to_string(i) +
                                                  ": " + e.what());
        } catch (const std::out_of_range& e) {
            logger::Logger::getInstance().warning("Skipping out-of-range DB row " +
                                                  std::to_string(i) + ": " + e.what());
        }
    }

    return tasks;
}

PgResult Repository::exec(std::string_view sql) {
    const std::string sql_str {sql};
    return PgResult {
        PQexec(conn_.get(), sql_str.c_str())}; // NOLINT(cppcoreguidelines-owning-memory)
}

PgResult Repository::execParams(std::string_view sql, const std::vector<const char*>& params) {
    const std::string sql_str {sql};
    return PgResult {PQexecParams( // NOLINT(cppcoreguidelines-owning-memory)
        conn_.get(),
        sql_str.c_str(),
        static_cast<int>(params.size()),
        nullptr,
        params.data(),
        nullptr,
        nullptr,
        0)};
}

} // namespace storage::db
