// #define _BSD_SOURCE 2387423
// clang -Wall -Wextra -std=c11 -g circle.c -o circle -lm -lGL -lglfw
// #include "core/Graphics.h"
#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>

#include "game/Game.h"
#include "core/Errors.h"
#include "core/Settings.h"
#include "core/Window.h"

int main(int argc, char **argv) {
    std::cout << "Starting!" << std::endl;

    try {
        Window::init();
    } catch(const WindowException& e) {
        std::cerr << e.what() << std::endl;
        Window::destroy();
        return 1;
    }

    Game game_core;
    game_core.init();
    
    while(Window::should_not_be_closed()) {
        game_core.loop();
    }

    Window::destroy();
    std::cout << "Ending with FPS: " << game_core.get_fps() << " Hz" << std::endl;
    return 0;
}


