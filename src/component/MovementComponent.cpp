#include "MovementComponent.h"
#include "Entity.h"


void MovementComponent::setDirection(float x, float y, float z) {
    setDirectionUnsafe(x, y, z);

    /* If nonzero normalize */
    if(direction.x != 0.0F || direction.y != 0.0F || direction.z != 0.0F) {
        direction.normalize();
    }
}

void MovementComponent::stop() {
    direction.x = 0.0F;
    direction.y = 0.0F;
    direction.z = 0.0F;
}

void MovementComponent::setDirectionUnsafe(float x, float y, float z) {
    direction.x = x;
    direction.y = y;
    direction.z = z;
}

void MovementComponent::update(float dt) {
    auto translation = getTranslation(dt);
    parent->getCuboidElevationSpace()->topLeft.x += translation.x;
    parent->getCuboidElevationSpace()->topLeft.y += translation.y;
    parent->getCuboidElevationSpace()->topLeft.z += translation.z;
    
}

Vect3F MovementComponent::getTranslation(float dt) const {
    return Vect3F{direction.x * speed * dt, direction.y * speed * dt, direction.z * speed * dt};
}
