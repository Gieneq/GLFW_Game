#include "MovementComponent.h"
#include "Entity.h"

void MovementComponent::setDirection(Direction dir) {
    if(dir == Direction::NORTH) {
        direction.x = 0.0F;
        direction.y = -1.0F;
    }
    else if(dir == Direction::SOUTH) {
        direction.x = 0.0F;
        direction.y = 1.0F;
    }
    else if(dir == Direction::WEST) {
        direction.x = -1.0F;
        direction.y = 0.0F;
    }
    else if(dir == Direction::EAST) {
        direction.x = 1.0F;
        direction.y = 0.0F;
    } 
    
    /* Stop */
    else {
        direction.x = 0.0F;
        direction.y = 0.0F;
    }
}

void MovementComponent::update(float dt) {
    auto translation = getTranslation(dt);
    parent->getPositionElevationSpace().x += translation.x;
    parent->getPositionElevationSpace().y += translation.y;
}

Vect2F MovementComponent::getTranslation(float dt) const {
    return Vect2F{direction.x * speed * dt, direction.y * speed * dt};
}
