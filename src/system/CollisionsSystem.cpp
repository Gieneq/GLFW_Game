#include "CollisionsSystem.h"
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include "Entity.h"
#include "World.h"
#include "Settings.h"

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
                auto collidedEntity = collisionCheckResult.getCollidedEntity();
                const auto& collidingCuboidElevationSpace = collisionCheckResult.getCollidingCuboidElevationSpace();
                collisonDetectorCmp->onCollision(collidedEntity, collidingCuboidElevationSpace);

                /* Append to recent results but in world space to easier render */
                const auto cuboidWorldSpace = entity->getContainingElevationOrThrow()->elevationToWorldSpace(collidingCuboidElevationSpace);
                collidingCuboidsWorldSpace.push_back(cuboidWorldSpace);
                

                /**
                 * On collision execution.
                 * 
                 * Here add logic to handle all collision related components.
                */
               onCollision(collisonDetectorCmp, collCmp, collidingCuboidElevationSpace);

               /* No need to check any other - will be done in next loop */
               return;
            }
        }
    }

    /* Seems no collision */
    if(entity->getCuboidElevationSpace().z() > 0.0) {

        /* Entity is in air */
        entity->getCuboidElevationSpace().z() = 0.0F;
    }
    
}

void CollisionsSystem::onCollision(CollisionDetectorComponent* collisionDetectorCmp, 
        CollisionComponent* collidedComponent, const Cuboid6F& collidedCuboidElevationSpace) {

        /**
         * Here check if Entity has some more collision related components.
         * Execute specific interactions to handle them.
        */

        if(collidedComponent->canWalkOn()) {
            /* Check if depth is acceptable */
            const auto obstacleDepth = collidedCuboidElevationSpace.depth();
            //todo - should be relative
            if(obstacleDepth < Settings::Systems::Collisions::MAX_WALKABLE_DEPTH) {
                /* Place entity ontop */
                auto parent = collisionDetectorCmp->getParentEntity();
                auto& parentElevationSpaceCuboid = parent->getCuboidElevationSpace();
                parentElevationSpaceCuboid.z() = collidedCuboidElevationSpace.z() + collidedCuboidElevationSpace.depth();
                return;
            }
        }

        /* Stop movement */
        auto movementCmp = collisionDetectorCmp->getParentEntity()->getComponent<MovementComponent>();
        const auto boundingCuboidElevationSpace = collisionDetectorCmp->getElevationSpaceBoundingCuboid();
        if(movementCmp) {
            stopMovingComponent(movementCmp, boundingCuboidElevationSpace, collidedCuboidElevationSpace);
       }
    }

void CollisionsSystem::stopMovingComponent(MovementComponent* movementCmp,
        const Cuboid6F& boundingCuboidElevationSpace, const Cuboid6F& collidedCuboidElevationSpace) {

    auto parent = movementCmp->getParentEntity();
    auto& parentElevationSpaceCuboid = parent->getCuboidElevationSpace();


    /* Align position to colliding cuboids */

    /* Align in X */
    if(movementCmp->getDirection().x > 0.0F) {
        parentElevationSpaceCuboid.alignToLeftOf(collidedCuboidElevationSpace);
    }

    else if(movementCmp->getDirection().x < 0.0F) {
        parentElevationSpaceCuboid.alignToRightOf(collidedCuboidElevationSpace);
    }

    /* Align in Y */
    if(movementCmp->getDirection().y > 0.0F) {
        parentElevationSpaceCuboid.alignToTopOf(collidedCuboidElevationSpace);
    }

    else if(movementCmp->getDirection().y < 0.0F) {
        parentElevationSpaceCuboid.alignToBottomOf(collidedCuboidElevationSpace);
        const auto diffY = parentElevationSpaceCuboid.y() - boundingCuboidElevationSpace.y();
        parentElevationSpaceCuboid.y() -= diffY;
    }

    
    /* Stop entity */
    movementCmp->stop();
}