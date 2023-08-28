#include "CollisionsSystem.h"
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include "Entity.h"
#include "World.h"
#include "Settings.h"
#include <algorithm>

void CollisionsSystem::init() {

}

void CollisionsSystem::processDetector(Entity* detectorEntity) {
    debugEntites.clear();

    /**
     * Prerequisites:
     * 1. Entity has CollisionDetectorComponent
     * 2. Entity has MovementComponent
     * 3. Entity has to be on some elevation
     */
    auto detectorCmp = detectorEntity->getComponent<CollisionDetectorComponent>();
    if(!detectorCmp) {
        return;
    }

    auto movementCmp = detectorEntity->getComponent<MovementComponent>();
    if(!movementCmp) {
        return;
    }

    Elevation* parentElevation = nullptr;
    try {
        parentElevation = detectorEntity->getContainingElevationOrThrow();
    } catch(const std::exception&) {  
        return;
    }

    /**
     * Step 1 - Examin tiles floor edge collision
    */

    auto standingOnTilesQuad = parentElevation->getFloorNearbyTilesQuad(detectorEntity->getCuboidElevationSpace().getFlatten());
    if(!standingOnTilesQuad.hasAnyTile()) {
        /**
         * Detector is in air, it could be caused becouse of:
         * 1. Entity is falling, by leaving edge of elevation
         * 2. Entity is hang in void
        */
       std::cout << "Flying. Todo soon..." << std::endl;
    }

    /* Here parentElevation could changed by detectors falling */
    try {
        parentElevation = detectorEntity->getContainingElevationOrThrow();
    } catch(const std::exception&) {
        return;
    }

    if(standingOnTilesQuad.hasMissingTile()) {
        /**
         * If under missing tiles are high enough obstacles,
         * let detector move.
         * If there is no lower elevation or no obstacles, or
         * not walkable obstacles - align to edge.
        */
        if(true) {
            /* Align to edge */
            alignEntityCuboidToTilesQuad(detectorEntity, &standingOnTilesQuad, movementCmp->getDirection().getXY());
        }
        else {
            /* Proceed */
            //todo
        }
    }

    /**
     * Step 2 - On collisionComponents
    */
    auto gotAnyCollision = false;

    //todo - probably can pack to function and reuse when placing object on ground - #flying
    for(auto obstacleIt = parentElevation->collisionComponentsRegisterBegin(); obstacleIt != parentElevation->collisionComponentsRegisterEnd(); ++obstacleIt) {
        auto obstacle = *obstacleIt;
        if(obstacle->getParentEntity() == detectorEntity) {
            /* Skip detector */
            continue;
        }

        const auto detectorBounding = detectorCmp->getElevationSpaceBoundingCuboid();
        const auto collisionCuboidsElevationSpace = obstacle->getElevationSpaceCollisionCuboids();
        for(const auto& collisionCuboidElevationSpace : collisionCuboidsElevationSpace) {
            if(collisionCuboidElevationSpace.checkIntersection(detectorBounding)) {
                /* Collision occured */
                gotAnyCollision = true;
                auto depthDiff = std::abs(collisionCuboidElevationSpace.z() - detectorBounding.z());
                /* if cannot walk on or depth is large - align */
                if(!obstacle->canWalkOn() || depthDiff >= Settings::Systems::Collisions::MAX_WALKABLE_DEPTH) {
                    /* Align to edge */


                    //todo - not really this way - watch out boundign relation
                    onCollisionAlignToEdge(detectorEntity->getCuboidElevationSpace(), detectorBounding, collisionCuboidElevationSpace, movementCmp->getDirection().getXY());
                }
                else {
                    /* Stand on top */
                    onCollisionStandOntop(detectorEntity->getCuboidElevationSpace(), collisionCuboidElevationSpace);
                }
            }
        }
    }

    if(!gotAnyCollision) {
        /* If flying try placing it on grond. First check lower obstacles and place ontop if any. */
        if(detectorEntity->getCuboidElevationSpace().z() > 0.0F) {
            /* Flying! */
            // auto& detectorElevationSpace = detectorEntity->getCuboidElevationSpace();
            detectorEntity->getCuboidElevationSpace().z() = 0.0F;

            /* Find all posible cuboids by */
        }
    }
    /**
     * Somthing else blah
    */
}

void CollisionsSystem::onCollisionAlignToEdge(Cuboid6F& detectorElevationSpace, const Cuboid6F boundignBoxElevationSpace, const Cuboid6F& obstacleElevationSpace, Vect2F direction) {
    /* Align in X */
    if(direction.x > 0.0F) {
        detectorElevationSpace.alignToLeftOf(obstacleElevationSpace);
    }

    else if(direction.x < 0.0F) {
        detectorElevationSpace.alignToRightOf(obstacleElevationSpace);
    }

    /* Align in Y */
    if(direction.y > 0.0F) {
        detectorElevationSpace.alignToTopOf(obstacleElevationSpace);
        detectorElevationSpace.y() -= boundignBoxElevationSpace.y() - detectorElevationSpace.y();
    }

    else if(direction.y < 0.0F) {
        detectorElevationSpace.alignToBottomOf(obstacleElevationSpace);
        // detectorElevationSpace.y() += boundignBoxElevationSpace.y() - detectorElevationSpace.y();
    }
}


void CollisionsSystem::onCollisionStandOntop(Cuboid6F& detectorElevationSpace, const Cuboid6F& obstacleElevationSpace) {
    detectorElevationSpace.z() = obstacleElevationSpace.z() + obstacleElevationSpace.depth();
}


void CollisionsSystem::alignEntityCuboidToTilesQuad(Entity* entity, TilesQuad* tilesQuad, Vect2F direction) {
    auto& cuboid = entity->getCuboidElevationSpace();

    /* In pairs left-right are in relation to direction, but coords are elevation space */
    auto directedTilesPair = tilesQuad->getFrontTilesRelativeToDirection(direction);
    const auto directedBounding = directedTilesPair.getBoundingRect();
    auto negatedTilesPair = tilesQuad->getFrontTilesRelativeToDirection(direction.getNegated());
    const auto negatedBounding = negatedTilesPair.getBoundingRect();

    if(direction.x > 0.0F) {
        if(directedTilesPair.hasAny()) {
            cuboid.x() = directedBounding.left() - cuboid.width();
        }
        else if(negatedTilesPair.hasAny()) {
            cuboid.x() = negatedBounding.right() - cuboid.width();
        }
    }
    else if(direction.x < 0.0F) {
        if(directedTilesPair.hasAny()) {
            cuboid.x() = directedBounding.right();
        }
        else if(negatedTilesPair.hasAny()) {
            cuboid.x() = negatedBounding.left();
        }
    }
    else if(direction.y > 0.0F) {
        if(directedTilesPair.hasAny()) {
            cuboid.y() = directedBounding.top() - cuboid.height();
        }
        else if(negatedTilesPair.hasAny()) {
            cuboid.y() = negatedBounding.bottom() - cuboid.height();
        }
    }
    else if(direction.y < 0.0F) {
        if(directedTilesPair.hasAny()) {
            cuboid.y() = directedBounding.bottom();
        }
        else if(negatedTilesPair.hasAny()) {
            cuboid.y() = negatedBounding.top();
        }
    }
}






std::vector<Cuboid6F> CollisionsSystem::getDebugResultsCuboidsWorldSpace() {
    std::vector<Cuboid6F> cuboids;
    
    for(auto entity : debugEntites) {
        cuboids.push_back(entity->getCuboidWorldSpace());
    }

    return cuboids;
}





// void CollisionsSystem::update(
//     std::vector<CollisionComponent*>::iterator collisionCmpsBegin,
//     std::vector<CollisionComponent*>::iterator collisionCmpsEnd,
//     Entity *entity, float dt) {

//     collisionResults.clear();

//     auto collisonDetectorCmp = entity->getComponent<CollisionDetectorComponent>();
//     if(collisonDetectorCmp) {
//         /* The entity has collision detector */
//         //todo sweep somehow most colllision checks
//         /* Test collision with all entities except considered entity with collision detector */
//         for (auto collCmpIt = collisionCmpsBegin; collCmpIt != collisionCmpsEnd; ++collCmpIt) {
//             auto collCmp = *collCmpIt;
//             const auto collisionCheckResult = collisonDetectorCmp->checkCollision(*collCmp);

//             if(collisionCheckResult) {
//                 /* Collision occured */
//                 collisionResults.push_back(collisionCheckResult);
//             }
//         }

//         /* If collected any collisions */
//         if(collisionResults.size() > 0) {
//             onCollision();
//         }

//         // /* Seems no collision */
//         // else if(entity->getCuboidElevationSpace().z() > 0.0) {

//         //     /* Entity is in air */
//         //     entity->getCuboidElevationSpace().z() = 0.0F;
//         // }

//         /**
//          * Exain nearby tiles - find elevation edges and align to them.
//          * Probably 3x3 cross (4  tiles) around detector should be fine.
//          * If in some place there is no tile, place there temporary obstacle wall,
//          * but if there is lower elevation, and there is tile hig enough to walk on,
//          * let plaer change elevation.
//         */

//         //todo 

//     }
// }

// void CollisionsSystem::onCollision() {
//     for(const auto& collisionResult : collisionResults) {
//         auto detector = collisionResult.getCollisionDetectorComponent();
//         auto obstacle = collisionResult.getCollidedComponent();

//         for(const auto& obstacleCuboidElevationSpace : collisionResult) {
//             //todo calc nearby tiles
//             const auto shouldBreak = reactToCollision(detector, obstacleCuboidElevationSpace);
//             if(shouldBreak) {
//                 break;
//             }
//         }
//     }
// }


// bool CollisionsSystem::reactToCollision(CollisionDetectorComponent* detector , const Cuboid6F& collidedCuboidElevationSpace) {
//     /**
//      * If obstacle is not so high stand on it.
//      * If ostacle is too high, align to it.
//     */

//     auto detectorBackHeight = detector->getParentEntity()->getCuboidElevationSpace().back();
//     auto obstacleFrontHeight = collidedCuboidElevationSpace.front();
//     auto heightDiff = obstacleFrontHeight - detectorBackHeight;
//     // std::cout << "heightDiff: " << heightDiff << std::endl;

//     if(heightDiff >= 0.0F) {
//         /* Obstacle if higher - check if detector can climb */
//         if(heightDiff < Settings::Systems::Collisions::MAX_WALKABLE_DEPTH) {
//             /* Climb */
//             detector->getParentEntity()->getCuboidElevationSpace().z() = obstacleFrontHeight;

//             /* Check if reaches next elevation */
//             if(detector->getParentEntity()->getCuboidElevationSpace().z() >= 0.99F) {
//                 /* Reached next elevation */
//                 detector->getParentEntity()->getCuboidElevationSpace().z() = 0.0F;
//                 const int next_elevation_index = detector->getParentEntity()->getContainingElevationOrThrow()->getIndex() + 1;
//                 const int elevations_count = detector->getParentEntity()->getContainingElevationOrThrow()->getContainingWorld().getElevationsCount();

//                 if(next_elevation_index < elevations_count) {
//                     detector->getParentEntity()->getContainingElevationOrThrow()->getContainingWorld().moveDynamicEntityToElevationOrThrow(detector->getParentEntity(), next_elevation_index);
//                     /* Rest of collisions are invalid - stop and break */
//                     return true;
//                 }
//             }
//         }
//         else {
//             /* Cannot climb - align to edge */
//             attemptAlignFaceOpposite(detector, collidedCuboidElevationSpace);
//         }
//     }
//     // else {
//     //     /* Obstacle is lower - check if detector can go down */
//     //     if(heightDiff > -Settings::Systems::Collisions::MAX_WALKABLE_DEPTH) {
//     //         /* Go down */
//     //         detector->getParentEntity()->getCuboidElevationSpace().z() = obstacleFrontHeight;
//     //     }
//     //     else {
//     //         /* Cannot go down, should align to edge */
//     //         attemptAlignFaceOpposite(detector, collidedCuboidElevationSpace);
//     //     }
//     // }
//     return false;
// }

// void CollisionsSystem::attemptAlignFaceOpposite(CollisionDetectorComponent* detector , const Cuboid6F& collidedCuboidElevationSpace) {
//     auto parent = detector->getParentEntity();
//     auto movementCmp = parent->getComponent<MovementComponent>();
//     if(!movementCmp) {
//         return;
//     }

//     auto& parentElevationSpaceCuboid = parent->getCuboidElevationSpace();
//     const auto boundingCuboidElevationSpace = detector->getElevationSpaceBoundingCuboid();
//     /* Align position to colliding cuboids */

//     /* Align in X */
//     if(movementCmp->getDirection().x > 0.0F) {
//         parentElevationSpaceCuboid.alignToLeftOf(collidedCuboidElevationSpace);
//     }

//     else if(movementCmp->getDirection().x < 0.0F) {
//         parentElevationSpaceCuboid.alignToRightOf(collidedCuboidElevationSpace);
//     }

//     /* Align in Y */
//     if(movementCmp->getDirection().y > 0.0F) {
//         parentElevationSpaceCuboid.alignToTopOf(collidedCuboidElevationSpace);
//     }

//     else if(movementCmp->getDirection().y < 0.0F) {
//         parentElevationSpaceCuboid.alignToBottomOf(collidedCuboidElevationSpace);
//         const auto diffY = parentElevationSpaceCuboid.y() - boundingCuboidElevationSpace.y();
//         parentElevationSpaceCuboid.y() -= diffY;
//     }
    
//     /* Stop entity */
//     movementCmp->stop();
// }



//     //     /**
//     //      * Here check if Entity has some more collision related components.
//     //      * Execute specific interactions to handle them.
//     //     */
//     //    auto collidedComponent = collisionResult.getCollidedComponent();
//     //    auto collidedCuboidElevationSpace = collisionResult.getCollidingCuboidElevationSpace();
//     //    auto collisionDetectorCmp = collisionResult.getCollisionDetectorComponent();

//     //     if(collidedComponent->canWalkOn()) {
//     //         /* Check if depth is acceptable */
//     //         const auto obstacleDepth = collidedCuboidElevationSpace.depth();
//     //         const auto entityZ  = collisionDetectorCmp->getParentEntity()->getCuboidElevationSpace().z();
//     //         const auto diffZ = std::abs(entityZ - collidedCuboidElevationSpace.z());
//     //         //todo - should be relative
//     //         if(diffZ < Settings::Systems::Collisions::MAX_WALKABLE_DEPTH) {
//     //             /* Place entity ontop */
//     //             auto parent = collisionDetectorCmp->getParentEntity();
//     //             auto& parentElevationSpaceCuboid = parent->getCuboidElevationSpace();
//     //             parentElevationSpaceCuboid.z() = collidedCuboidElevationSpace.z() + collidedCuboidElevationSpace.depth();
//     //             return;
//     //         }
//     //     }

//     //     /* Stop movement */
//     //     auto movementCmp = collisionDetectorCmp->getParentEntity()->getComponent<MovementComponent>();
//     //     const auto boundingCuboidElevationSpace = collisionDetectorCmp->getElevationSpaceBoundingCuboid();
//     //     if(movementCmp) {
//     //         stopMovingComponent(movementCmp, boundingCuboidElevationSpace, collidedCuboidElevationSpace);
//     //    }
//     // }

// // void CollisionsSystem::stopMovingComponent(MovementComponent* movementCmp,
// //         const Cuboid6F& boundingCuboidElevationSpace, const Cuboid6F& collidedCuboidElevationSpace) {

// //     auto parent = movementCmp->getParentEntity();
// //     auto& parentElevationSpaceCuboid = parent->getCuboidElevationSpace();


// //     /* Align position to colliding cuboids */

// //     /* Align in X */
// //     if(movementCmp->getDirection().x > 0.0F) {
// //         parentElevationSpaceCuboid.alignToLeftOf(collidedCuboidElevationSpace);
// //     }

// //     else if(movementCmp->getDirection().x < 0.0F) {
// //         parentElevationSpaceCuboid.alignToRightOf(collidedCuboidElevationSpace);
// //     }

// //     /* Align in Y */
// //     if(movementCmp->getDirection().y > 0.0F) {
// //         parentElevationSpaceCuboid.alignToTopOf(collidedCuboidElevationSpace);
// //     }

// //     else if(movementCmp->getDirection().y < 0.0F) {
// //         parentElevationSpaceCuboid.alignToBottomOf(collidedCuboidElevationSpace);
// //         const auto diffY = parentElevationSpaceCuboid.y() - boundingCuboidElevationSpace.y();
// //         parentElevationSpaceCuboid.y() -= diffY;
// //     }

    
// //     /* Stop entity */
// //     movementCmp->stop();
// // }


// std::vector<Cuboid6F> CollisionsSystem::getCollisionResultsCuboidsWorldSpace() {
//     std::vector<Cuboid6F> collisionResultsWorldSpace;

//     for(const auto& collisionResult : collisionResults) {
//         for(const auto& cuboidElevationSpace : collisionResult) {
//             const auto cuboidWorldSpace = collisionResult.getCollidedComponent()->getParentEntity()->getContainingElevationOrThrow()->elevationToWorldSpace(cuboidElevationSpace);
//             collisionResultsWorldSpace.push_back(cuboidWorldSpace);
//         }
//         // const auto& collidingCuboidElevationSpace = collisionResult.getCollidingCuboidElevationSpace();
//         // const auto cuboidWorldSpace = collisionResult.getCollidedEntity()->getContainingElevationOrThrow()->elevationToWorldSpace(collidingCuboidElevationSpace);
//         // collisionResultsWorldSpace.push_back(cuboidWorldSpace);
//     }

//     return collisionResultsWorldSpace;
// }