#pragma once

#include "calculator.hpp"

namespace app {

class Application {
public:
    Application();
    ~Application();

    Application(const Application& other);
    Application& operator=(const Application& other);

    Application(Application&& other) noexcept;
    Application& operator=(Application&& other) noexcept;

    void run(int argc, char** argv);

private:
    Calculator calculator_;
};

} // namespace app
