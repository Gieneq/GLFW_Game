#include "Player.h"
#include "MovementComponent.h"
#include "CollisionComponents.h"
#include "ControllableComponent.h"
#include "GraphicsComponent.h"
#include "World.h"



MovementComponent* Player::addMovementComponent(float speed) {
    auto cmp = Entity::addMovementComponent(speed);
    this->movementComponent = cmp;
    return cmp;
}

std::optional<CollisionDetectorComponent*> Player::addCollisionDetectorComponent(const Rect2F& boundingRect) {
    auto cmpOption = Entity::addCollisionDetectorComponent(boundingRect);\
    if (cmpOption.has_value()) {
        this->collisionDetectorComponent = cmpOption.value();
    }
    return cmpOption;
}

std::optional<ControllableComponent*> Player::addControllableComponent() {
    auto cmpOption = Entity::addControllableComponent();
    if (cmpOption.has_value()) {
        this->controllableComponent = cmpOption.value();
    }
    return cmpOption;
}