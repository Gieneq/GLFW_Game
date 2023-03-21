#include "Errors.h"

const char* WindowException::what() const noexcept {
        return msg.c_str();
}