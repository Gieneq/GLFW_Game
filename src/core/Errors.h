#pragma once
#include <string>
#include <exception>

class WindowException : public std::exception {
public:
    WindowException(const std::string& msg) : msg{msg} {}
    const char* what() const noexcept override;

private:
    std::string msg{};
};