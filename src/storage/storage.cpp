#include "storage.hpp"

#include <stdexcept>
#include <string>

#include "../config/config.hpp"
#include "../logger/logger.hpp"

namespace storage {

Storage::Storage() {
    try {
        const config::Config cfg {"config.json"};
        const std::string conninfo = cfg.db().toConnString();
        repo_.emplace(conninfo);
        warmUp();
    } catch (const std::runtime_error& e) {
        logger::Logger::getInstance().warning(
            std::string {"Storage unavailable, running in cache-only mode: "} + e.what());
        repo_.reset();
    }
}

std::optional<calculator::Task> Storage::find(const calculator::Task& task) const {
    return cache_.find(task);
}

void Storage::save(const calculator::Task& task) {
    cache_.insert(task);

    if (repo_) {
        try {
            repo_->save(task);
        } catch (const std::runtime_error& e) {
            logger::Logger::getInstance().error(std::string {"Failed to persist task to DB: "} +
                                                e.what());
        }
    }
}

bool Storage::isDbAvailable() const noexcept {
    return repo_.has_value();
}

void Storage::warmUp() {
    if (!repo_) {
        return;
    }

    logger::Logger::getInstance().info("Warming up cache from DB...");
    const std::vector<calculator::Task> records = repo_->loadAll();

    for (const auto& task : records) {
        cache_.insert(task);
    }

    logger::Logger::getInstance().info("Cache warm-up complete: " + std::to_string(cache_.size()) +
                                       " records loaded");
}

} // namespace storage
