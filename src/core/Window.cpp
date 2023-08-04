#include "Window.h"
#include <vector>
#include "GLCommon.h"
#include "Callbacks.h"
#include "Errors.h"
#include "Settings.h"

static GLFWwindow *window;

/**
 * List of key click receivers. If keybord's key is clicked, 
 * listeners callback function will be executed.
 * 
 * TODO - some fort of prioritazing, sorting, disabling 
 * and receive response (bool return)
*/
static std::vector<KeyboardEvent*> keyboard_callbacks;

static void onErrorCallback(int error, const char *desc) {
        fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    // See http://www.glfw.org/docs/latest/group__keys.html
    if ((key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, GL_TRUE);
        return;
    }

    if (action == GLFW_PRESS) {
        for(auto keyboard_callback : keyboard_callbacks) {
            keyboard_callback->on_key_press(key);
            //todo eval bool
        }
    }
    
    if (action == GLFW_RELEASE) {
        for(auto keyboard_callback : keyboard_callbacks) {
            keyboard_callback->on_key_release(key);
            //todo eval bool
        }
    }
}

void Window::addKeyboardListener(KeyboardEvent* ke) {
    keyboard_callbacks.push_back(ke);
}

void Window::init() {
    glfwSetErrorCallback(onErrorCallback);

    if (!glfwInit()) {
        throw WindowException("Cannot init GLFW!");
    }
    
    window = glfwCreateWindow(Settings::Window::WIDTH, Settings::Window::HEIGHT, Settings::Window::TITLE, NULL, NULL);
    if (window == nullptr) {
        throw WindowException("Cannot create window!");
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if(Settings::Window::ENABLE_VSYNC) {
        glfwSwapInterval(1);
    }
}

bool Window::shouldNotBeClosed() {
    return !glfwWindowShouldClose(window);
}

void Window::prepare() {
    glClearColor(Settings::Window::ClearColor::r,
    Settings::Window::ClearColor::g,
    Settings::Window::ClearColor::b,
    Settings::Window::ClearColor::a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::destroy() {
    if(window) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}

int Window::width() {
    int window_width;
    glfwGetFramebufferSize(window, &window_width, NULL);
    return window_width;
}

int Window::height() {
    int window_height;
    glfwGetFramebufferSize(window, NULL, &window_height);
    return window_height;
}

