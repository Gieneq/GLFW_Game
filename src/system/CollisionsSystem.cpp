#include "CollisionsSystem.h"
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include "Entity.h"
#include "World.h"
#include "Settings.h"
#include <algorithm>
#include <array>

void CollisionsSystem::init() {

}


void CollisionsSystem::startSession() {
    lastResults.clear();
    debugEntites.clear();
    walkableCuboids.clear();
    notWalkableCuboids.clear();
}

void CollisionsSystem::processDetector(Entity* detectorEntity, const CollisionComponentsList::const_iterator& collisionComponentsBegin, 
    const CollisionComponentsList::const_iterator& collisionComponentsEnd) {
        auto requirements = CollisionsSystem::checkDetectororRequirements(detectorEntity);
        if(!requirements.has_value()) {
            return;
        }

        auto [detectorCmp, movementCmp, parentElevation] = requirements.value();

        auto floorEntitiesUnderDetector = parentElevation->getEntitiesIntersetingWith(Entity::Type::FLOOR, detectorEntity->getCuboid().value().getFlatten());
        debugEntites.insert(debugEntites.end(), floorEntitiesUnderDetector.begin(), floorEntitiesUnderDetector.end());

        const bool hasFloorUnder = floorEntitiesUnderDetector.size() > 0;


        /* Elevating UP */

        if(detectorEntity->getCuboid().value().z() > 0.999F) {
            const float nextZ = detectorEntity->getCuboid().value().z() - 1.0F;
            const int nextElevationIdx = parentElevation->getIndex() + 1;
            std::cout << "Switching to elevation: " << nextElevationIdx << std::endl;
            try {
                /* Elevating Down */
                parentElevation->getContainingWorld().moveEntityToElevationOrThrow(detectorEntity, nextElevationIdx);
                detectorEntity->getCuboid().value().z() = nextZ;
                return;
            } catch(const std::exception&) {
                /* No lower elevations */
                detectorEntity->getCuboid().value().z() = 0.0F;
            }
        }

        /* Movement check */
        const float dt = 0.1F;
        if(detectorEntity->getCuboid().value().z() > 0.0F || !hasFloorUnder) {
            detectorEntity->getCuboid().value().z() -= 2.0F * dt;
            if(detectorEntity->getCuboid().value().z() < 0.0F) {
                if(!hasFloorUnder) {
                    const float nextZ = detectorEntity->getCuboid().value().z() + 1.0F;
                    const int nextElevationIdx = parentElevation->getIndex() - 1;
                    std::cout << "Switching to elevation: " << nextElevationIdx << std::endl;
                    try {
                        /* Elevating Down */
                        parentElevation->getContainingWorld().moveEntityToElevationOrThrow(detectorEntity, nextElevationIdx);
                        detectorEntity->getCuboid().value().z() = nextZ;
                        return;
                    } catch(const std::exception&) {
                        /* No lower elevations */
                        detectorEntity->getCuboid().value().z() = 0.0F;
                    }
                }
                else {
                    detectorEntity->getCuboid().value().z() = 0.0F;
                }
            }
        }
        
        /* Assumption - detetorBoundingZ = entityCuboidZ */

        /* Collison check and reaction */

        auto results = detectorCmp->checkCollision(collisionComponentsBegin, collisionComponentsEnd);

        /**
         * Results are truely colliding components.
        */

        if (results.size() > 0) {
            /* Has some collisions */
            debugEntites.push_back(detectorEntity);

            for (auto& result : results) {
                auto [collisionCmp, collisionCuboids] = result;

                debugEntites.push_back(collisionCmp->getParentEntity());

                /* Case 1 - not walkable */
                if(!collisionCmp->canWalkOn()) {
                    for (const auto& collisionCuboid : collisionCuboids) {
                        notWalkableCuboids.push_back(collisionCuboid);
                        CollisionsSystem::leanAgainstObstacle(detectorCmp, movementCmp->getDirection().getXY(), collisionCuboid);
                    }
                }

                /* Case 2 - walkable */
                else {
                    for (const auto& collisionCuboid : collisionCuboids) {
                        const float heightDiff = collisionCuboid.value().z() - detectorCmp->getElevationBoundingCuboid().value().z();

                        const bool isTooLow = false;//heightDiff < -Settings::Systems::Collisions::MAX_WALKABLE_DEPTH;
                        const bool isTooHigh = false;//heightDiff > Settings::Systems::Collisions::MAX_WALKABLE_DEPTH;

                        /* Case 2.1 - walkable, but too high */
                        if(isTooLow || isTooHigh) {
                            notWalkableCuboids.push_back(collisionCuboid);
                            CollisionsSystem::leanAgainstObstacle(detectorCmp, movementCmp->getDirection().getXY(), collisionCuboid);
                        }
                        /* Case 2.2 - walkable, not too high */
                        else {
                            walkableCuboids.push_back(collisionCuboid);
                            CollisionsSystem::standOnObstacle(detectorCmp, collisionCuboid);
                        }
                    }
                }
            }
        }
        // else {
        //     /* Has no collisions - chack if outside elevation */

        //     auto floorEntitiesUnderDetector = parentElevation->getFloorEntitiesInRect(detectorEntity->getCuboid().value().getFlatten());
        //     debugEntites.insert(debugEntites.end(), floorEntitiesUnderDetector.begin(), floorEntitiesUnderDetector.end());

        //     bool isAboveGround = detectorEntity->getCuboid().value().z() > 0.0F;
        //     bool hasFloorUnder = floorEntitiesUnderDetector.size() > 0;

        //     /* Case 3 - remains on floor */
        //     if(hasFloorUnder) {
        //         /* Case 3.1 - is above the ground - place on enetity or ground */

        //         /* Case 3.1.1 - place on entity */
        //         if(isAboveGround) {
        //             auto collisionCmpUnderDetector = parentElevation->getCollisionComponentsInRect(detectorEntity->getCuboid().value().getFlatten());

        //             if(collisionCmpUnderDetector.size() > 0) {

        //                 auto elevationCuboids = CollisionComponent::retriveElevationCuboidsFromComponents(collisionCmpUnderDetector.begin(), collisionCmpUnderDetector.end());
        //                 const auto& tallestElevationCuboid = ElevationCuboid::getMaxSide<Side::FRONT>(elevationCuboids.begin(), elevationCuboids.end());
        //                 CollisionsSystem::standOnObstacle(detectorCmp, tallestElevationCuboid);
        //             }
                                    
        //             /* Case 3.1.2 - place on ground */
        //             else {
        //                 detectorEntity->getCuboid().value().z() = 0.0F;
        //             }
        //         }
                    
        //         /* Case 3.1 - remains on floor */
        //         else {
        //             // std::cout << "Todo: Touching ground. Nothing" << std::endl;
        //         }

        //     }

        //     /* Case 4 - outside elevation */
        //     else {
        //         bool hasLowerElevation = false;
        //         /* Case 4.1 - has lower elevation - fall */
        //         if(hasLowerElevation) {
        //             std::cout << "Todo: falling down like on stairs..." << std::endl;
        //         }

        //         /* Case 4.2 - is outside the world */
        //         else {
        //             std::cout << "Todo: outside the world. Not important" << std::endl;
        //         }
        //     }
        // }
    }


std::optional<CollisionsSystem::InitialRequirements> CollisionsSystem::checkDetectororRequirements(Entity* detectorEntity) {
    auto detectorCmp = detectorEntity->getComponent<CollisionDetectorComponent>();
    if(!detectorCmp) {
        return std::nullopt;
    }

    auto movementCmp = detectorEntity->getComponent<MovementComponent>();
    if(!movementCmp) {
        return std::nullopt;
    }

    Elevation* parentElevation = nullptr;
    try {
        parentElevation = detectorEntity->getContainingElevationOrThrow();
    } catch(const std::exception&) {  
        return std::nullopt;
    }

    return std::make_tuple(detectorCmp, movementCmp, parentElevation);
}

void CollisionsSystem::leanAgainstObstacle(CollisionDetectorComponent* detectorCmp, const Vect2F& direction, const ElevationCuboid& obstacle) {
    auto& parentCuboid = detectorCmp->getParentEntity()->getCuboid().value();
    const auto boundingCuboid = detectorCmp->getElevationBoundingCuboid().value();
    Cuboid6F::alignParentCuboidToOther(parentCuboid, boundingCuboid, obstacle.value(), direction);
}


void CollisionsSystem::standOnObstacle(CollisionDetectorComponent* detectorCmp, const ElevationCuboid& obstacle) {
    auto& parentCuboid = detectorCmp->getParentEntity()->getCuboid().value();
    const auto boundingCuboid = detectorCmp->getElevationBoundingCuboid().value();
    Cuboid6F::placeParentCuboidOnOther(parentCuboid, boundingCuboid, obstacle.value());
}

std::vector<WorldCuboid> CollisionsSystem::getDebugResults() {
    std::vector<WorldCuboid> cuboids;
    for(auto entity : debugEntites) {
        cuboids.push_back(entity->getCuboid().toWorldSpace());
    }
    return cuboids;
}

std::vector<WorldCuboid> CollisionsSystem::getWalkableObstaclesCuboids() {
    std::vector<WorldCuboid> cuboids;
    for(auto& cuboid : walkableCuboids) {
        cuboids.push_back(cuboid.toWorldSpace());
    }
    return cuboids;
}

std::vector<WorldCuboid> CollisionsSystem::getNotWalkableObstaclesCuboids() {
    std::vector<WorldCuboid> cuboids;
    for(auto& cuboid : notWalkableCuboids) {
        cuboids.push_back(cuboid.toWorldSpace());
    }
    return cuboids;
}



