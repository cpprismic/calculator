#pragma once

#include <stdexcept>
#include <string>

namespace app {

class AppException : public std::runtime_error {
public:
    explicit AppException(const std::string& message) : std::runtime_error(message) {}
};

class ParseException : public AppException {
public:
    explicit ParseException(const std::string& message) : AppException(message) {}
};

class CalculationException : public AppException {
public:
    explicit CalculationException(const std::string& message) : AppException(message) {}
};

} // namespace app