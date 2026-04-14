#include <cstdio>
#include <stdexcept>

#include "app/application.hpp"
#include "logger/logger.hpp"

int main(int argc, char** argv) {
    try {
        auto& log = logger::Logger::getInstance();

#ifdef APP_DEBUG
        log.setLevel(logger::LogLevel::Debug);
        log.debug("Build type: Debug -- log level set to DEBUG");
#else
        log.setLevel(logger::LogLevel::Info);
#endif

        app::Application application;
        application.run(argc, argv);

    } catch (const std::invalid_argument& e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    } catch (const std::runtime_error& e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 1;
    } catch (const std::exception& e) {
        fprintf(stderr, "Unexpected error: %s\n", e.what());
        return 1;
    }

    return 0;
}
