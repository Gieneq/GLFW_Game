#include "Window.h"
// #include "glad/gl.h"
// // #endif
// #define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <vector>

#include "Callbacks.h"

static GLFWwindow *window;
static int window_width{Settings::Window::WIDTH};
static int window_height{Settings::Window::HEIGHT};
static std::vector<KeyboardEvent*> keyboard_callbacks;



static void error(int error, const char *desc) {
        fputs(desc, stderr);
}

//todo make vector of callback handlers - attach them, sort somehow

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

void Window::add_keyboard_listener(KeyboardEvent* ke) {
    keyboard_callbacks.push_back(ke);
}

void Window::init() {
    glfwSetErrorCallback(error);

    if (!glfwInit()) {
        throw WindowException("Cannot init GLFW!");
    }
    
    window = glfwCreateWindow(Settings::Window::WIDTH, Settings::Window::HEIGHT, Settings::Window::TITLE, NULL, NULL);
    if (window == nullptr) {
        throw WindowException("Cannot create window!");
    }

    
    glfwMakeContextCurrent(window);
    // gladLoadGL(glfwGetProcAddress);
    glfwSetKeyCallback(window, key_callback);

    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &window_width, &window_height);
}

bool Window::should_not_be_closed() {
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
        return window_width;
    }

    int Window::height() {
        return window_height;
    }


//         int width, height;
//         glfwGetFramebufferSize(w, &width, &height);

//                 // Actual OpenGL calls
//                 glClear(GL_COLOR_BUFFER_BIT);
//                 glDisable(GL_DEPTH_TEST);
//                 glMatrixMode(GL_PROJECTION);
//                 glLoadIdentity();
//                 glOrtho(0, width, height, 0, 0, 1);
//                 glMatrixMode(GL_MODELVIEW);

//                 // Draw our line
//                 glBegin(GL_LINE_LOOP);
// //                        int x1 = 100, y1 = 100;
// //                        int x2 = 200, y2 = 200;
// //
//                         glColor3f(0, 0, 1);

//                         // Framerate-based angel. As the frames are displayed, this integer will increment
//                         // and continue until it hits 360, then it will wrap around and start from 0
//                         static double iteration = 0;
//                         // The x, y offset onto the screen -- this should later be centered
//                         static const int offset = 150;
//                         // The radius of both our circle and the circle it's spinning in.
//                         static const float radius = 50;

//                         // Calculate our x, y cooredinates
//                         double x1 = offset + radius + 100 * cos(iteration);
//                         double y1 = offset + radius + 100 * sin(iteration);
//                         static double wobble = 0.0;
//                         //double x2 = offset + 100 + radius * cos(iteration), y2 = offset + 100 * sin(iteration);
//                         //
//                         y1 += sin(wobble) * 100;
//                         wobble += 0.01;

//                         // A = (π * r²)
//                         double a = M_PI * (100 * 2);
//                         // C = (2 * π * r)
//                         double c = 2 * M_PI * 100;

//                         static double b = 128;
//                         //b += 1.0;
//                         for (double i = 0; i < 2 * M_PI; i = i + ((2 * M_PI) / b))
//                         {
// //                                glVertex2f(x1 + sin(i) * M_PI, y1 + cos(i) * M_PI);

//                                 glVertex2f(x1 + radius * cos(i), y1 + radius * sin(i));
// //                                glVertex2f(x1, y1);
// //                                glVertex2f(x2, y2);
//                         }

//                         iteration += 0.01;

//                 glEnd();


        //         glfwSwapBuffers(w);
        //         glfwPollEvents();
        // }