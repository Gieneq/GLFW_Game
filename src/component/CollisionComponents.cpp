#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <iostream>
#include "Entity.h"
#include "World.h"

/* CollisionComponent */

std::vector<Cuboid6F> CollisionComponent::getElevationSpaceCollisionCuboids() const {
    const auto& parentElevationSpacePosition = parent->getCuboidElevationSpace().topLeft;
    std::vector<Cuboid6F> elevationSpaceCollisionCuboids;
    for (const auto& cuboid : collisionCuboids) {
        elevationSpaceCollisionCuboids.push_back(cuboid.getTranslated(parentElevationSpacePosition));
    }
    return elevationSpaceCollisionCuboids;
}

std::vector<Cuboid6F> CollisionComponent::getWorldSpaceCollisionCuboids() const {
    const auto& parentWorldSpacePosition = parent->getCuboidWorldSpace().topLeft;
    std::vector<Cuboid6F> worldSpaceCollisionCuboids;
    for (const auto& cuboid : collisionCuboids) {
        worldSpaceCollisionCuboids.push_back(cuboid.getTranslated(parentWorldSpacePosition));
    }
    return worldSpaceCollisionCuboids;
}

/* CollisionDetectorComponent */

CollisionResult CollisionDetectorComponent::checkCollision(CollisionComponent& other) {
    /* Filter out check with self */
    if (other.getParentEntity()->getId() == parent->getId()) {
        return CollisionResult::NONE();
    }

    auto result = CollisionResult(this, &other);

    const auto otherCollisionElevationSpaceCuboids = other.getElevationSpaceCollisionCuboids();
    const auto thisBoundingElevationSpaceCuboids = getElevationSpaceBoundingCuboid();

    for (const auto& otherCuboidElevationSpace : otherCollisionElevationSpaceCuboids) {
        if (thisBoundingElevationSpaceCuboids.checkIntersection(otherCuboidElevationSpace)) {
            // return CollisionResult{this, &other, other.getParentEntity(), otherCuboidElevationSpace};
            // #error
            result.addCollidedCuboid(otherCuboidElevationSpace);
        }
    }
    
    return result;
}

Cuboid6F CollisionDetectorComponent::getElevationSpaceBoundingCuboid() const {
    const auto& parentElevationSpacePosition = parent->getCuboidElevationSpace().topLeft;
    return boundingCuboid.getTranslated(parentElevationSpacePosition);
}

Cuboid6F CollisionDetectorComponent::getWorldSpaceBoundingCuboid() const {
    const auto& parentWorldSpacePosition = parent->getCuboidWorldSpace().topLeft;
    return boundingCuboid.getTranslated(parentWorldSpacePosition);
}