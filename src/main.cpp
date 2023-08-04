#include "GLCommon.h"
#include <iostream>
#include "Game.h"
#include "Errors.h"
#include "Settings.h"
#include "Window.h"
#include "Loader.h"

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

    /**
     * Using default OpenGL, but it could be done
     * somehow better. See GLAD stuff.
     * gladLoadGL(glfwGetProcAddress);
    */
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    
    game.init();
    
    while(Window::shouldNotBeClosed()) {
        game.loop();
    }

    Window::destroy();
    std::cout << "Closing!" << std::endl;
    return 0;
}
