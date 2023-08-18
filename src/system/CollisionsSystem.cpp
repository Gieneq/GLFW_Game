#include "CollisionsSystem.h"
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include "Entity.h"
#include "World.h"

void CollisionsSystem::init() {

}


void CollisionsSystem::update(
    std::vector<CollisionComponent*>::iterator collisionCmpsBegin,
    std::vector<CollisionComponent*>::iterator collisionCmpsEnd,
    Entity *entity, float dt) {

    collidingCuboidsWorldSpace.clear();

    auto collisonDetectorCmp = entity->getComponent<CollisionDetectorComponent>();
    if(collisonDetectorCmp) {
        /* The entity has collision detector */
        //todo sweep somehow most colllision checks
        /* Test collision with all entities except considered entity with collision detector */
        for (auto collCmpIt = collisionCmpsBegin; collCmpIt != collisionCmpsEnd; ++collCmpIt) {
            auto collCmp = *collCmpIt;
            const auto collisionCheckResult = collisonDetectorCmp->checkCollision(*collCmp);

            if(collisionCheckResult) {
                collisonDetectorCmp->onCollision(collisionCheckResult.getCollidedEntity(), collisionCheckResult.begin(), collisionCheckResult.end());

                /* Append to recent results but in world space to easier render */
                for( const auto& cuboidElevationSpace : collisionCheckResult) {
                    const auto cuboidWorldSpace = entity->getContainingElevationOrThrow()->elevationToWorldSpace(cuboidElevationSpace);
                    collidingCuboidsWorldSpace.push_back(cuboidWorldSpace);
                }

                /**
                 * On collision execution.
                 * 
                 * Here add logic to handle all collision related components.
                */
               onCollision(collisonDetectorCmp, collisionCheckResult.getCollidedEntity(), collisionCheckResult.begin(), collisionCheckResult.end());
            }
        }
    }
    
}

void CollisionsSystem::onCollision(
    CollisionDetectorComponent* collisionDetectorCmp,
    Entity* collidedEntity, 
    std::vector<Cuboid6F>::const_iterator elevationSpaceCuboidsBegin, 
    std::vector<Cuboid6F>::const_iterator elevationSpaceCuboidsEnd) {

        /**
         * Here check if Entity has some more collision related components.
         * Execute specific interactions to handle them.
        */

       //todo examin objects depth to eventualy stand on them

        /* Stop movement */
        auto movementCmp = collisionDetectorCmp->getParentEntity()->getComponent<MovementComponent>();
        const auto boundingCuboidElevationSpace = collisionDetectorCmp->getElevationSpaceBoundingCuboid();
        if(movementCmp) {
            stopMovingComponent(movementCmp, boundingCuboidElevationSpace, elevationSpaceCuboidsBegin, elevationSpaceCuboidsEnd);
       }
    }

void CollisionsSystem::stopMovingComponent(MovementComponent* movementCmp,
    const Cuboid6F& boundingCuboidElevationSpace,
    std::vector<Cuboid6F>::const_iterator elevationSpaceCuboidsBegin, 
    std::vector<Cuboid6F>::const_iterator elevationSpaceCuboidsEnd) {

    auto parent = movementCmp->getParentEntity();
    auto& parentElevationSpaceCuboid = parent->getCuboidElevationSpace();

    /* Align in X direction */
    // if(movementCmp->direction.x > 0) {
    //     movementCmp->direction.x = 0;
    //     parent->getCuboidElevationSpace().topLeft.x = rect.left() - boundingCuboidElevationSpace.width();
    //     //todo offset to fit bounding box
    // }

    // else if(movementCmp->direction.x < 0) {
    //     movementCmp->direction.x = 0;
    //     parent->getCuboidElevationSpace().topLeft.x = rect.right();
    //     //todo offset to fit bounding box
    // }

    /* Align in Y direction */
    // if(movementCmp->direction.y > 0) {
    //     movementCmp->direction.y = 0;
    //     parent->getCuboidElevationSpace().topLeft.y = rect.top() - (boundingRect.size.h + boundingRect.topLeft.y);
    // }

    // else if(movementCmp->direction.y < 0) {
    //     movementCmp->direction.y = 0;
    //     parent->getCuboidElevationSpace().topLeft.y = rect.bottom();
    //     float diffY = boundingRect.top();
    //     parent->getCuboidElevationSpace().topLeft.y -= diffY;
    // }
}