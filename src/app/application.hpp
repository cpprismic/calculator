#pragma once

#include "../storage/storage.hpp"
#include "calculator.hpp"

namespace app {

class Application {
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    void run(int argc, char** argv);

private:
    calculator::Calculator calculator_;
    storage::Storage storage_;
};

} // namespace app
