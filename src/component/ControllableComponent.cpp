#include "ControllableComponent.h"
#include "MovementComponent.h"
#include "World.h"
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
        controlledMovement->setDirectionUnsafe(0.0F, 0.0F, 0.0F);
    } 
    else if(lastPressedKey == MovementControlKey::UP) {
        controlledMovement->setDirectionUnsafe(0.0F, -1.0F, 0.0F);
    }
    else if(lastPressedKey == MovementControlKey::DOWN) {
        controlledMovement->setDirectionUnsafe(0.0F, 1.0F, 0.0F);
    }
    else if(lastPressedKey == MovementControlKey::LEFT) {
        controlledMovement->setDirectionUnsafe(-1.0F, 0.0F, 0.0F);
    }
    else if(lastPressedKey == MovementControlKey::RIGHT) {
        controlledMovement->setDirectionUnsafe(1.0F, 0.0F, 0.0F);
    }
}

void ControllableComponent::onPressSpaceKey() {
}

void ControllableComponent::onReleaseSpaceKey() {
    std::cout << "Attempt to jump up!" << std::endl;
    
    auto parent = getParentEntity();
    auto elevation = parent->getContainingElevationOrThrow();
    auto& world = elevation->getContainingWorld();

    /* Check if next layer exists */
    if(elevation->getIndex() + 1 >= world.getElevationsCount()) {
        return;
    }
    world.moveEntityToElevationOrThrow(parent, elevation->getIndex() + 1);
}

void ControllableComponent::onPressLShiftKey() {

}

void ControllableComponent::onReleaseLShiftKey() {
    std::cout << "Attempt to jump down!" << std::endl;

    auto parent = getParentEntity();
    auto elevation = parent->getContainingElevationOrThrow();
    auto& world = elevation->getContainingWorld();

    /* Check if next layer exists */
    if(elevation->getIndex() -1 < 0) {
        return;
    }
    world.moveEntityToElevationOrThrow(parent, elevation->getIndex() - 1);
}
