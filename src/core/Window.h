#pragma once

class KeyboardEvent;

namespace Window
{
    void init();
    bool shouldNotBeClosed();
    void prepare();
    void update();
    void destroy();

    int width();
    int height();
    void addKeyboardListener(KeyboardEvent* ke);
} // namespace Window
