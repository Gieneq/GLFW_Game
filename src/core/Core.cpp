#include "Core.h"

void Core::init() {
    last_time = glfwGetTime();
    frame_count = 0;
}


void Core::loop() {
    double current_time = glfwGetTime();
    double delta_time = current_time - last_time;
    last_time = current_time;

    input();
    update(static_cast<float>(delta_time));
    render();

    frame_count++;
    if (delta_time >= Settings::General::FPS_MEASURING_INTERVAL) {
        fps_evaluated = frame_count / delta_time;
        frame_count = 0;
    }

}

double Core::get_fps() const {
    return fps_evaluated;
}