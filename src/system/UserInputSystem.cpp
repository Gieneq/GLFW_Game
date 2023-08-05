#include "UserInputSystem.h"
#include "GLCommon.h"
#include "Entity.h"
#include "ControllableComponent.h"

void UserInputSystem::init() {

}

void UserInputSystem::attachControllabe(Entity *entity) {
    controllableObject = entity->getComponent<ControllableComponent>();
    if(controllableObject) {
        std::cout << "Controllable object attached!" << std::endl;
    }
}

bool UserInputSystem::onKeyPressed(int key) {
    // std::cout << "Key pressed: " << key << std::endl;
    if(controllableObject) {
        if(key == GLFW_KEY_W) {
            controllableObject->onPressWSADKey(GLFW_KEY_W);
            return true;
        } else if(key == GLFW_KEY_A) {
            controllableObject->onPressWSADKey(GLFW_KEY_A);
            return true;
        } else if(key == GLFW_KEY_S) {
            controllableObject->onPressWSADKey(GLFW_KEY_S);
            return true;
        } else if(key == GLFW_KEY_D) {
            controllableObject->onPressWSADKey(GLFW_KEY_D);
            return true;
        }   
    }
    return false;
}

bool UserInputSystem::onKeyReleased(int key) {
    // std::cout << "Key released: " << key << std::endl;
    if(controllableObject) {
        if(key == GLFW_KEY_W) {
            controllableObject->onReleaseWSADKey(GLFW_KEY_W);
            return true;
        } else if(key == GLFW_KEY_A) {
            controllableObject->onReleaseWSADKey(GLFW_KEY_A);
            return true;
        } else if(key == GLFW_KEY_S) {
            controllableObject->onReleaseWSADKey(GLFW_KEY_S);
            return true;
        } else if(key == GLFW_KEY_D) {
            controllableObject->onReleaseWSADKey(GLFW_KEY_D);
            return true;
        }
    }
    return false;
}
