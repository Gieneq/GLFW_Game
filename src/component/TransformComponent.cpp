#include "TransformComponent.h"

void TransformComponent::update(float dt) {
    if(moving) {
        target.x += direction.x * speed * dt;
        target.y += direction.y * speed * dt;
    }
}

void TransformComponent::direction_north() {
    direction.x = 0.0F;
    direction.y = -1.0F;
}

void TransformComponent::direction_south() {
    direction.x = 0.0F;
    direction.y = 1.0F;
}

void TransformComponent::direction_west() {
    direction.x = -1.0F;
    direction.y = 0.0F;
}

void TransformComponent::direction_east() {
    direction.x = 1.0F;
    direction.y = 0.0F;
}

void TransformComponent::go() {
    moving = true;
}

void TransformComponent::stop() {
    moving = false;
}