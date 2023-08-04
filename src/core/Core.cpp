#include "Core.h"
#include <iostream>
#include "GLCommon.h"
#include "Window.h"
#include "Settings.h"

void Core::init() {
    lastTime = glfwGetTime();
    frameCount = 0;
}


void Core::loop() {
    double current_time = glfwGetTime();
    double delta_time = current_time - lastTime;
    accumulatedTime += delta_time;
    lastTime = current_time;

    input();
    update(static_cast<float>(delta_time));
    
    Window::prepare();
    render();
    Window::update();

    frameCount++;
    if (accumulatedTime >= Settings::General::FPS_MEASURING_INTERVAL) {
        calculatedFPS = static_cast<double>(frameCount) / accumulatedTime;
        accumulatedTime = 0;
        frameCount = 0;
    }

}

double Core::getFPS() const {
    return calculatedFPS;
}