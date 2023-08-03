#include "UserInputSystem.h"


// #include "glad/glad.h"
#include "GLFW/glfw3.h"
// #include "glad/glad.h"
// #define GLFW_INCLUDE_NONE
// #include "GLFW/glfw3.h"

void UserInputSystem::init() {

}

void UserInputSystem::attach_controllabe(Entity *entity) {
    if(!this->controllable_entity) {
        if(entity->hasComponent<WSADControllableComponent>()) {
            this->controllable_entity = entity;
            wsad_controllable = entity->getComponent<WSADControllableComponent>();
        }
    }
    //todo some feedback
}

bool UserInputSystem::on_key_press(int key) {
    if(wsad_controllable) {
        if(key == GLFW_KEY_W) {
            wsad_controllable->press_w();
            return true;
        } else if(key == GLFW_KEY_A) {
            wsad_controllable->press_a();
            return true;
        } else if(key == GLFW_KEY_S) {
            wsad_controllable->press_s();
            return true;
        } else if(key == GLFW_KEY_D) {
            wsad_controllable->press_d();
            return true;
        }
    }
    return false;
}

bool UserInputSystem::on_key_release(int key) {
    if(wsad_controllable) {
        if(key == GLFW_KEY_W) {
            wsad_controllable->release_w();
            return true;
        } else if(key == GLFW_KEY_A) {
            wsad_controllable->release_a();
            return true;
        } else if(key == GLFW_KEY_S) {
            wsad_controllable->release_s();
            return true;
        } else if(key == GLFW_KEY_D) {
            wsad_controllable->release_d();
            return true;
        }
    }
    return false;
}

    // nk_glfw3_font_stash_begin(&atlas);
    // nk_glfw3_font_stash_end();

    // glfwSetKeyCallback(window, key_callback);
    // glfwSetCursorPosCallback(window, cursor_pos_callback);