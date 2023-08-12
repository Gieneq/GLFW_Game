#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <iostream>
#include "Entity.h"

std::vector<Rect2F> CollisionComponent::getElevationSpaceCollisionRects() const {
    std::vector<Rect2F> elevationSpaceCollisionRects;
    for (auto& rect : collisionRects) {
        elevationSpaceCollisionRects.push_back(rect.get_translated(parent->getPositionElevationSpace()));
    }
    return elevationSpaceCollisionRects;
}

std::vector<Rect3F> CollisionComponent::getWorldSpaceCollisionRects() const {
    std::vector<Rect3F> worldSpaceCollisionRects;
    for (auto& esRect : getElevationSpaceCollisionRects()) {
        auto worldSpaceRect = Rect3F(
            esRect.left(),
            esRect.top(),
            parent->getWorldSpaceZ(),
            esRect.size.w,
            esRect.size.h,
            parent->getLength()
        );
        worldSpaceCollisionRects.push_back(worldSpaceRect);
    }
    return worldSpaceCollisionRects;
}

void CollisionDetectorComponent::onCollision(std::vector<Rect2F> collidingRectsElevationSpace) {
    // std::cout << "[!] Collision of " << getWorldSpaceBoundingRect() << "detected with:";
    // for (auto& rect : collidingRects) {
    //     std::cout << rect << ", " << std::endl;
    // }
    
    /**
     * React to collision:
     * - stop moving towards each collidingRect,
     * - align to each collidingRect,
     */
    for (auto& rect : collidingRectsElevationSpace) {

        /* Align horizontally */
        if(movementCmp->direction.x > 0) {
            movementCmp->direction.x = 0;
            parent->setXElevationSpace(rect.left() - boundingRect.size.w);
            //todo offset to fit bounding box
        } else if(movementCmp->direction.x < 0) {
            movementCmp->direction.x = 0;
            parent->setXElevationSpace(rect.right());
            //todo offset to fit bounding box
        }

        /* Align vertically */
        if(movementCmp->direction.y > 0) {
            movementCmp->direction.y = 0;
            parent->setYElevationSpace(rect.top() - boundingRect.size.h);
            float diffY = parent->getSize().h - boundingRect.bottom();
            parent->addYElevationSpace(diffY);
        } else if(movementCmp->direction.y < 0) {
            movementCmp->direction.y = 0; 
            parent->setYElevationSpace(rect.bottom());
            float diffY = boundingRect.top();
            parent->addYElevationSpace(-diffY);
        }
    }
}

Rect2F CollisionDetectorComponent::getElevationSpaceBoundingRect() const {
    return boundingRect.get_translated(parent->getPositionElevationSpace());
}