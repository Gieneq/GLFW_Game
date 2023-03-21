#pragma once
// #include "Graphics.h"
// #include <GLFW/glfw3.h>

#include "Errors.h"
#include "Settings.h"

namespace Window
{
    void init();
    bool should_not_be_closed();
    void prepare();
    void update();
    void destroy();

    int width();
    int height();
} // namespace Window
