#include "Core.h"
#include <iostream>
#include <cmath>
#include "GLCommon.h"
#include "Window.h"
#include "Settings.h"
#include "Timers.h"

bool Core::init() {
    lastTime = glfwGetTime();
    frameCount = 0;
    return true;
}

void Core::loop() {
    double current_time = glfwGetTime();
    double delta_time = current_time - lastTime;
    int deltaTImeMs = static_cast<int>(delta_time * 1000.0);
    FPSAccumulatedTime += delta_time;
    logAccumulatedTime += delta_time;
    lastTime = current_time;

    input();
    update(static_cast<float>(delta_time));
    TimersRegister::getTimersRegister().update(deltaTImeMs);
    
    Window::prepare();
    render();
    Window::update();
    
    logState();
}

void Core::appendLogMessage(std::string && str) {
    logMessages.push_back(std::move(str));
}

void Core::logState() {
    frameCount++;
    if (FPSAccumulatedTime >= Settings::General::FPS_MEASURE_INTERVAL) {
        calculatedFPS = static_cast<double>(frameCount) / FPSAccumulatedTime;
        FPSAccumulatedTime -= Settings::General::LOG_STATE_INTERVAL;
        frameCount = 0;
    }

    if (logAccumulatedTime >= Settings::General::LOG_STATE_INTERVAL) {
        logAccumulatedTime -= Settings::General::LOG_STATE_INTERVAL;

        if(Settings::General::LOG_STATE) {
            /* Build LOG message */
            std::cout << "[Core " << std::round(getTime() * 100.0) / 100.0 << " s] " << "FPS: " << std::round(getFPS() * 10.0) / 10.0 << " Hz" << 
            ", usage: " << std::round(Window::getUsagePercentage() * 10.0) / 10.0 << "%";
            
            if(logMessages.empty()) {
                std::cout << std::endl;
            }
            else {
                std::cout << ", messages: " << std::endl;
                for(const auto& msg : logMessages) {
                    std::cout << " - " << msg << std::endl;
                }
            }
        }
        logMessages.clear();
    }
}

double Core::getFPS() const {
    return calculatedFPS;
}

double Core::getTime() const {
    return glfwGetTime();
}