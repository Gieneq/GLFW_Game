#include "CollisionsSystem.h"
#include "CollisionComponents.h"
#include "Entity.h"

void CollisionsSystem::init() {

}


void CollisionsSystem::update(const std::vector<CollisionComponent*>& collisionCmps, Entity *entity, float dt) {
    collidingRects.clear();
    //todo sweep somehow most colllision checks
    auto collisonDetectorCmp = entity->getComponent<CollisionDetectorComponent>();
    if(collisonDetectorCmp) {
        /* The entity has collision detector */
        const auto boundingRectWorldSpace = collisonDetectorCmp->getWorldSpaceBoundingRect();

        /* Test collision with all entities except considered entity with collision detector */
        for (auto collCmp : collisionCmps) {
            auto otherEntityOption = collCmp->getParent();
            if(otherEntityOption.has_value() && (otherEntityOption.value()->id != entity->id)) {
                /* Other entity has collision compoinent */
                auto otherCollisionRects = collCmp->getWorldSpaceCollisionRects();
                for (auto& otherCollisionRect : otherCollisionRects) {
                    if(boundingRectWorldSpace.checkIntersection(otherCollisionRect)) {
                        collidingRects.push_back(otherCollisionRect);
                    }
                }
            }
        }

        if(collidingRects.size() > 0) {
            collisonDetectorCmp->onCollision(collidingRects);
            
            //to debug render
            collidingRects.push_back(boundingRectWorldSpace);
        }
    }
    
}