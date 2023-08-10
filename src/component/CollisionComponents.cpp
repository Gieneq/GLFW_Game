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
            //todo offset to fit bounding box
        } else if(movementCmp->direction.x < 0) {
            movementCmp->direction.x = 0;
            movementCmp->parentLocation->worldRect.alignToRightOf(rect);
            //todo offset to fit bounding box
        }

        /* Align vertically */
        if(movementCmp->direction.y > 0) {
            movementCmp->direction.y = 0;
            movementCmp->parentLocation->worldRect.alignToTopOf(rect);
            float diffY = movementCmp->parentLocation->worldRect.size.h - boundingRect.bottom();
            movementCmp->parentLocation->worldRect.top_left.y += diffY;
        } else if(movementCmp->direction.y < 0) {
            movementCmp->direction.y = 0; 
            movementCmp->parentLocation->worldRect.alignToBottomOf(rect);
            float diffY = boundingRect.top_left.y;
            movementCmp->parentLocation->worldRect.top_left.y -= diffY;
        }
    }
}

Rect2F CollisionDetectorComponent::getWorldSpaceBoundingRect() const {
    return boundingRect.get_translated(movementCmp->parentLocation->worldRect.top_left);
}