#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <iostream>
#include "Entity.h"
#include "World.h"

std::vector<Rect2F> CollisionComponent::getElevationSpaceCollisionRects() const {
    Vect2F parentElevationSpacePosition = parent->getCuboidElevationSpace().topLeft.getXY();
    std::vector<Rect2F> elevationSpaceCollisionRects;
    for (auto& rect : collisionRects) {
        elevationSpaceCollisionRects.push_back(rect.getTranslated(parentElevationSpacePosition));
    }
    return elevationSpaceCollisionRects;
}

std::vector<Rect3F> CollisionComponent::getWorldSpaceCollisionRects() const {
    std::vector<Rect3F> worldSpaceCollisionRects;
    for (auto& esRect : getElevationSpaceCollisionRects()) {
        auto worldSpaceRect = Rect3F(
            esRect.topLeft.x,
            esRect.topLeft.y,
            parent->getContainingElevationOrThrow()->getWorldSpaceZ(),
            esRect.size.w,
            esRect.size.h,
            0.0F
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
            parent->getCuboidElevationSpace().topLeft.x = rect.left() - boundingRect.size.w;
            //todo offset to fit bounding box
        } 
        
        else if(movementCmp->direction.x < 0) {
            movementCmp->direction.x = 0;
            parent->getCuboidElevationSpace().topLeft.x = rect.right();
            //todo offset to fit bounding box
        }

        /* Align vertically */
        if(movementCmp->direction.y > 0) {
            movementCmp->direction.y = 0;
            parent->getCuboidElevationSpace().topLeft.y = rect.top() - (boundingRect.size.h + boundingRect.topLeft.y);
        } 
        
        else if(movementCmp->direction.y < 0) {
            movementCmp->direction.y = 0; 
            parent->getCuboidElevationSpace().topLeft.y = rect.bottom();
            float diffY = boundingRect.top();
            parent->getCuboidElevationSpace().topLeft.y -= diffY;
        }
    }
}

Rect2F CollisionDetectorComponent::getElevationSpaceBoundingRect() const {
    Vect2F parentElevationSpacePosition = parent->getCuboidElevationSpace().topLeft.getXY();
    return Rect2F(
        boundingRect.topLeft.x + parentElevationSpacePosition.x,
        boundingRect.topLeft.y + parentElevationSpacePosition.y,
        boundingRect.size.w,
        boundingRect.size.h
    );
}

Rect3F CollisionDetectorComponent::getWorldSpaceBoundingRect() const {
    auto elevationSpaceBoundingRect = getElevationSpaceBoundingRect();
    return Rect3F(
        elevationSpaceBoundingRect.topLeft.x,
        elevationSpaceBoundingRect.topLeft.y,
        parent->getContainingElevationOrThrow()->getWorldSpaceZ(),
        elevationSpaceBoundingRect.size.w,
        elevationSpaceBoundingRect.size.h,
        0.0F
    );
}