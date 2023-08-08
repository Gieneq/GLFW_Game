#include "CollisionComponents.h"
#include "LocationComponent.h"
#include "MovementComponent.h"
#include <iostream>

std::vector<Rect2F> CollisionComponent::getWorldSpaceCollisionRects() const {
    std::vector<Rect2F> worldSpaceCollisionRects;
    for (auto& rect : collisionRects) {
        worldSpaceCollisionRects.push_back(rect.get_translated(parentLocation->worldRect.top_left));
    }
    return worldSpaceCollisionRects;
}

void CollisionDetectorComponent::onCollision(std::vector<Rect2F> collidingRects) {
    // std::cout << "Collision of " << getWorldSpaceBoundingRect() << "detected with:" << std::endl;
    // for (auto& rect : collidingRects) {
    //     std::cout << " --> " << rect << std::endl;
    // }
    // std::cout << std::endl;
    // movementCmp->stop();
}

Rect2F CollisionDetectorComponent::getWorldSpaceBoundingRect() const {
    return boundingRect.get_translated(movementCmp->parentLocation->worldRect.top_left);
}