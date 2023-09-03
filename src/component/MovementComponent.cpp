#include "MovementComponent.h"
#include "Entity.h"


void MovementComponent::setDirection(float x, float y, float z) {
    setDirectionUnsafe(x, y, z);

    /* If nonzero normalize */
    if(direction.x != 0.0F || direction.y != 0.0F || direction.z != 0.0F) {
        direction.normalize();

        /* Save nonzero direction */
        heading = direction;
    }
}

void MovementComponent::stop() {
    setDirectionUnsafe(0.0F, 0.0F, 0.0F);
}

void MovementComponent::setDirectionUnsafe(float x, float y, float z) {
    direction.x = x;
    direction.y = y;
    direction.z = z;
}

void MovementComponent::update(float dt) {
    auto translation = getTranslation(dt);
    parent->getCuboid().value().translate(translation);
    
}

Vect3F MovementComponent::getTranslation(float dt) const {
    return Vect3F{direction.x * speed * dt, direction.y * speed * dt, direction.z * speed * dt};
}
