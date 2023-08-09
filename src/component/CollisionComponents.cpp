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
    // std::cout << "[!] Collision of " << getWorldSpaceBoundingRect() << "detected with:";
    // for (auto& rect : collidingRects) {
    //     std::cout << rect << ", " << std::endl;
    // }
    
    /**
     * React to collision:
     * - stop moving towards each collidingRect,
     * - align to each collidingRect,
     */
    for (auto& rect : collidingRects) {

        /* Align horizontally */
        if(movementCmp->direction.x > 0) {
            movementCmp->direction.x = 0;
            movementCmp->parentLocation->worldRect.alignToLeftOf(rect);
        } else if(movementCmp->direction.x < 0) {
            movementCmp->direction.x = 0;
            movementCmp->parentLocation->worldRect.alignToRightOf(rect);
        }

        /* Align vertically */
        if(movementCmp->direction.y > 0) {
            movementCmp->direction.y = 0;
            movementCmp->parentLocation->worldRect.alignToTopOf(rect);
        } else if(movementCmp->direction.y < 0) {
            movementCmp->direction.y = 0; //todo - hacky, this works other way, try make the same
            movementCmp->parentLocation->worldRect.alignToBottomOf(rect);
        }
    }
}

Rect2F CollisionDetectorComponent::getWorldSpaceBoundingRect() const {
    return boundingRect.get_translated(movementCmp->parentLocation->worldRect.top_left);
}