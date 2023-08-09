#pragma once

class KeyboardEventListener;

namespace Window
{
    void init();
    bool shouldNotBeClosed();
    void prepare();
    void update();
    void destroy();

    int width();
    int height();
    double getUsagePercentage();

    void addKeyboardListener(KeyboardEventListener* ke);
} // namespace Window
