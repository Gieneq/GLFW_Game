#include "Player.h"
#include "LocationComponent.h"
#include "MovementComponent.h"
#include "CollisionComponents.h"
#include "ControllableComponent.h"
#include "GraphicsComponent.h"
#include "World.h"

bool Player::isOnFloor() const {
    return getFloor().has_value();
}

std::optional<Floor*> Player::getFloor() const {
    if(!locationComponent) {
        return std::nullopt;
    }

    if(!locationComponent->containingFloor) {
        return std::nullopt;
    }

    return locationComponent->containingFloor;
}


