#include "CollisionsSystem.h"
#include "CollisionComponents.h"
#include "Entity.h"

void CollisionsSystem::init() {

}


void CollisionsSystem::update(
    std::vector<CollisionComponent*>::iterator collisionCmpsBegin,
    std::vector<CollisionComponent*>::iterator collisionCmpsEnd,
    Entity *entity, float dt) {

    /**
     * Check is done in elevation space - 
     * obstacles and entity are on the same elevation.
     * No need to check Z axis.
     * Resulting Rect2F are also in elevation space.
    */

    collidingRects.clear();

    //todo sweep somehow most colllision checks
    auto collisonDetectorCmp = entity->getComponent<CollisionDetectorComponent>();
    if(collisonDetectorCmp) {
        /* The entity has collision detector */
        const auto boundingRectElevationSpace = collisonDetectorCmp->getElevationSpaceBoundingRect();

        /* Test collision with all entities except considered entity with collision detector */
        for (auto collCmpIt = collisionCmpsBegin; collCmpIt != collisionCmpsEnd; ++collCmpIt) {
            auto collCmp = *collCmpIt;
            auto otherEntity = collCmp->getParentEntity();
            if(otherEntity && (otherEntity->getId() != entity->getId())) {
                /* Other entity has collision compoinent */
                auto otherCollisionRects = collCmp->getElevationSpaceCollisionRects();
                for (auto& otherCollisionRect : otherCollisionRects) {
                    if(boundingRectElevationSpace.checkIntersection(otherCollisionRect)) {
                        collidingRects.push_back(otherCollisionRect);
                    }
                }
            }
        }

        if(collidingRects.size() > 0) {
            collisonDetectorCmp->onCollision(collidingRects);
            
            //to debug render
            collidingRects.push_back(collisonDetectorCmp->getElevationSpaceBoundingRect());
        }
    }
    
}