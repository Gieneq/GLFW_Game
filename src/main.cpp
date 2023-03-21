// #define _BSD_SOURCE 2387423
// clang -Wall -Wextra -std=c11 -g circle.c -o circle -lm -lGL -lglfw
#include "main.h"

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
        Window::update();
    }

    Window::destroy();
    std::cout << "Ending!" << std::endl;
    return 0;
}


