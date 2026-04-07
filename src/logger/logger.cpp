#include "logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace logger {

struct Logger::Impl {
    std::shared_ptr<spdlog::logger> logger;

    Impl() {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.log", false);

        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        logger = std::make_shared<spdlog::logger>("app", sinks.begin(), sinks.end());
        logger->set_level(spdlog::level::debug);
    }
};

Logger::Logger() : impl_(std::make_unique<Impl>()) {}
Logger::~Logger() = default;

Logger& Logger::getInstance() {
    static Logger inst;
    return inst;
}

void Logger::setLevel(LogLevel level) {
    switch (level) {
    case LogLevel::Debug:
        impl_->logger->set_level(spdlog::level::debug);
        return;
    case LogLevel::Info:
        impl_->logger->set_level(spdlog::level::info);
        return;
    case LogLevel::Warning:
        impl_->logger->set_level(spdlog::level::warn);
        return;
    case LogLevel::Error:
        impl_->logger->set_level(spdlog::level::err);
        return;
    }
}

void Logger::log(LogLevel level, std::string_view message) {
    switch (level) {
    case LogLevel::Debug:
        impl_->logger->debug(message);
        break;
    case LogLevel::Info:
        impl_->logger->info(message);
        break;
    case LogLevel::Warning:
        impl_->logger->warn(message);
        break;
    case LogLevel::Error:
        impl_->logger->error(message);
        break;
    }
}

void Logger::debug(std::string_view message) {
    log(LogLevel::Debug, message);
}
void Logger::info(std::string_view message) {
    log(LogLevel::Info, message);
}
void Logger::warning(std::string_view message) {
    log(LogLevel::Warning, message);
}
void Logger::error(std::string_view message) {
    log(LogLevel::Error, message);
}

} // namespace logger
