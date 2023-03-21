#pragma once
#include <GLFW/glfw3.h>

#include "Errors.h"
#include "Settings.h"

namespace Window
{
    void init();
    bool should_not_be_closed();
    void update();
    void destroy();
} // namespace Window
