#include "Window.h"


static GLFWwindow *window;
static void error(int error, const char *desc) {
        fputs(desc, stderr);
}

static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
        // See http://www.glfw.org/docs/latest/group__keys.html
        if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
                glfwSetWindowShouldClose(w, GL_TRUE);
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
    glfwSetKeyCallback(window, key_callback);
}

bool Window::should_not_be_closed() {
    return !glfwWindowShouldClose(window);
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