#include "GLCommon.h"
#include <iostream>
#include "game/Game.h"
#include "core/Errors.h"
#include "core/Settings.h"
#include "core/Window.h"

static Game game;

int main(int argc, char **argv) {
    std::cout << "Starting!" << std::endl;

    try {
        Window::init();
    } 
    catch(const WindowException& e) {
        std::cerr << e.what() << std::endl;
        Window::destroy();
        return 1;
    }

    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;


    game.init();
    
    while(Window::shouldNotBeClosed()) {
        game.loop();
    }

    Window::destroy();
    std::cout << "Ending with FPS: " << game.getFPS() << " Hz" << std::endl;
    return 0;
}
