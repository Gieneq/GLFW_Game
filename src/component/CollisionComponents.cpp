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

CollisionResults CollisionDetectorComponent::checkCollision(const CollisonCmpsVector::const_iterator& begin, const CollisonCmpsVector::const_iterator& end) const {
    auto result = CollisionResults(this);
    for (auto collisionCmpIt = begin; collisionCmpIt != end; ++collisionCmpIt) {
        auto collisionCmp = *collisionCmpIt;

        /* Filter out check with self */
        if (collisionCmp->getParentEntity()->getId() == parent->getId()) {
            continue;
        }

        const auto thisBounding = getElevationBoundingCuboid();
        const auto otherCollisionElevationCuboids = collisionCmp->getElevationCollisionCuboids();

        for (const auto& otherCuboid : otherCollisionElevationCuboids) {
            auto elevationCollidingCuboids = std::vector<ElevationCuboid>{};
            if (thisBounding.checkIntersection(otherCuboid)) {
                elevationCollidingCuboids.push_back(otherCuboid);
            }
            result.addCollidedCuboids(collisionCmp, elevationCollidingCuboids.cbegin(), elevationCollidingCuboids.cend());
        }
    }
    
    return result;
}

ElevationCuboid CollisionDetectorComponent::getElevationBoundingCuboid() const {
    return ElevationCuboid::transformLocalCuboid(parent->getCuboid(), boundingCuboid);
}