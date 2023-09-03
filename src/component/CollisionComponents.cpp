#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <iostream>
#include "Entity.h"
#include "World.h"

/* CollisionComponent */

std::vector<ElevationCuboid> CollisionComponent::getElevationCollisionCuboids() const {
    return ElevationCuboid::transformLocalCuboids(parent->getCuboid(), collisionCuboids.begin(), collisionCuboids.end());
}

std::vector<WorldCuboid> CollisionComponent::getWorldCollisionCuboids() const {
    return WorldCuboid::transformLocalCuboids(parent->getCuboid(), collisionCuboids.begin(), collisionCuboids.end());
}

/* CollisionDetectorComponent */

CollisionResults checkCollision(const CollisonCmpsVector::const_iterator& begin, const CollisonCmpsVector::const_iterator& end) const {
    /* Filter out check with self */
    if (other.getParentEntity()->getId() == parent->getId()) {
        return CollisionResult::NONE();
    }

    auto result = CollisionResult(this, &other);

    const auto collisionElevationCuboids = other.get

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

ElevationCuboid CollisionDetectorComponent::getElevationBoundingCuboid() const {
    return ElevationCuboid::transformLocalCuboid(parent->getCuboid(), boundingCuboid);
}