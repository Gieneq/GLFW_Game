#pragma once
// #include "Graphics.h"
#include <iostream>
#include <GLFW/glfw3.h>

#include "Errors.h"
#include "Settings.h"

class Core {
public:
    virtual ~Core() = default;
    void init();
    void loop();

    virtual bool input() {return false;};
    virtual void update(float dt) {};
    virtual void render() {};
    double get_fps() const;

private:
    double last_time{0.0};
    double accumulated_time{0.0};
    int frame_count{0};
    double fps_evaluated{0.0};
};