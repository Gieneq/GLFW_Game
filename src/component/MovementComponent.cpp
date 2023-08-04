#include "MovementComponent.h"
#include "LocationComponent.h"

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
    else {
        direction.x = 1.0F;
        direction.y = 0.0F;
    }
}

void MovementComponent::update(float dt) {
    auto translation = getTranslation(dt);
    parentLocation->worldRect.top_left.x += translation.x;
    parentLocation->worldRect.top_left.y += translation.y;
}

Vect2F MovementComponent::getTranslation(float dt) const {
    return Vect2F{direction.x * speed * dt, direction.y * speed * dt};
}

// void TransformComponent::update(float dt) {
//     if(moving) {
//         target.x += direction.x * speed * dt;
//         target.y += direction.y * speed * dt;
//     }
// }

// void TransformComponent::direction_north() {
//     direction.x = 0.0F;
//     direction.y = -1.0F;
// }

// void TransformComponent::direction_south() {
//     direction.x = 0.0F;
//     direction.y = 1.0F;
// }

// void TransformComponent::direction_west() {
//     direction.x = -1.0F;
//     direction.y = 0.0F;
// }

// void TransformComponent::direction_east() {
//     direction.x = 1.0F;
//     direction.y = 0.0F;
// }

// void TransformComponent::go() {
//     moving = true;
// }

// void TransformComponent::stop() {
//     moving = false;
// }