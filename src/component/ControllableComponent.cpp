#include "ControllableComponent.h"
#include "MovementComponent.h"
#include "Entity.h"
#include "GLCommon.h" 

ControllableComponent::ControllableComponent(Entity* e, MovementComponent* move) : Component(e), controlledMovement(move) {
    std::unordered_map<int, bool> pressedKeysWSAD {
        {GLFW_KEY_W, false},
        {GLFW_KEY_S, false},
        {GLFW_KEY_A, false},
        {GLFW_KEY_D, false}
    };
}   

void ControllableComponent::onPressWSADKey(int key) {
    if(!active) {
        return;
    }

    if(key == GLFW_KEY_W) {
        lastPressedKey = MovementControlKey::UP;
    }
    else if(key == GLFW_KEY_S) {
        lastPressedKey = MovementControlKey::DOWN;
    }
    else if(key == GLFW_KEY_A) {
        lastPressedKey = MovementControlKey::LEFT;
    }
    else if(key == GLFW_KEY_D) {
        lastPressedKey = MovementControlKey::RIGHT;
    }
    else {
        std::cerr << "Unknown key pressed in ControllableComponent: " << key << std::endl;
    }

    pressedKeysWSAD[key] = true;
    updateMovement();
}

void ControllableComponent::onReleaseWSADKey(int key) {
    if(!active) {
        return;
    }

    if(key == GLFW_KEY_W && lastPressedKey == MovementControlKey::UP) {
        lastPressedKey = MovementControlKey::NONE;
    }
    else if(key == GLFW_KEY_S && lastPressedKey == MovementControlKey::DOWN) {
        lastPressedKey = MovementControlKey::NONE;
    }
    else if(key == GLFW_KEY_A && lastPressedKey == MovementControlKey::LEFT) {
        lastPressedKey = MovementControlKey::NONE;
    }
    else if(key == GLFW_KEY_D && lastPressedKey == MovementControlKey::RIGHT) {
        lastPressedKey = MovementControlKey::NONE;
    }

    pressedKeysWSAD[key] = false;
    updateMovement();
}

void ControllableComponent::updateMovement() {
    /* Update direction */
    if(!controlledMovement) {
        return;
    }

    if(lastPressedKey == MovementControlKey::NONE) {
        controlledMovement->setDirection(Direction::NONE);
    } 
    else if(lastPressedKey == MovementControlKey::UP) {
        controlledMovement->setDirection(Direction::NORTH);
    }
    else if(lastPressedKey == MovementControlKey::DOWN) {
        controlledMovement->setDirection(Direction::SOUTH);
    }
    else if(lastPressedKey == MovementControlKey::LEFT) {
        controlledMovement->setDirection(Direction::WEST);
    }
    else if(lastPressedKey == MovementControlKey::RIGHT) {
        controlledMovement->setDirection(Direction::EAST);
    }
}