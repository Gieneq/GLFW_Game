#include "Core.h"
#include "Window.h"

void Core::init() {
    last_time = glfwGetTime();
    frame_count = 0;
}


void Core::loop() {
    double current_time = glfwGetTime();
    double delta_time = current_time - last_time;
    accumulated_time += delta_time;
    last_time = current_time;

    input();
    update(static_cast<float>(delta_time));
    
    Window::prepare();
    render();
    Window::update();

    frame_count++;
    if (accumulated_time >= Settings::General::FPS_MEASURING_INTERVAL) {
        fps_evaluated = static_cast<double>(frame_count) / accumulated_time;
        accumulated_time = 0;
        frame_count = 0;
    }

}

double Core::get_fps() const {
    return fps_evaluated;
}