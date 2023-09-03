#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include "Coordinates.h"
#include <vector>
#include <tuple>

class CollisionDetectorComponent;
class CollisionComponent;


/**
 * Collision result for collision check with all valid objects.
 * All colliding components with their filtered cuboids in elevation space
 * are transfered.
*/
enum class CollisionAbility {
    WALKABLE,
};

constexpr collisionAbilityToInt(CollisionAbility ability) {
    switch (ability) {
        case CollisionAbility::WALKABLE:
            return 0;
        default:
            return -1;
    }
}

class CollisionResults {
public:
    using ResultsVector = std::vector<std::tuple<CollisionComponent*, std::vector<ElevationCuboid>>>;

    CollisionResults(CollisionDetectorComponent* collisionDetectorCmp, const ResultsVector::const_iterator& begin, 
        const ResultsVector::const_iterator& end) : 
        collisionDetectorCmp{collisionDetectorCmp}, collisionResults{begin, end} {}

    operator bool() const {
        return size() > 0;
    }

    inline std::vector<CollisionComponent*> getCollidedComponents() const {
        std::vector<CollisionComponent*> collidedComponents;
        for (const auto& result : collisionResults) {
            collidedComponents.push_back(std::get<0>(result));
        }
        return collidedComponents;
    }

    inline std::vector<ElevationCuboid> getCollidedCuboids() const {
        std::vector<ElevationCuboid> collidedCuboids;
        for (const auto& result : collisionResults) {
            collidedCuboids.insert(collidedCuboids.end(), std::get<1>(result).begin(), std::get<1>(result).end());
        }
        return collidedCuboids;
    }

    CollisionDetectorComponent* getCollisionDetectorComponent() const {
        return collisionDetectorCmp;
    }

    ResultsVector::const_iterator begin() const {
        return collisionResults.begin();
    }

    ResultsVector::const_iterator end() const {
        return collisionResults.end();
    }

    size_t size() const {
        return collisionResults.size();
    }

    inline int getCollidedComponentsCount() const {
        return static_cast<int>(collisionResults.size());
    }

    static CollisionResults NONE() {
        return CollisionResults{nullptr, {}, {}};
    }

    /* Because some Cuboids of Component can not collide */
    inline void addCollidedCuboid(CollisionComponent* collisionComponent,
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end) {
            const auto cuboids = ElevationCuboid::transformLocalCuboids(collisionComponent->getParentEntity()->getCuboid(), begin, end);
        collisionResults.push_back(std::make_tuple(collisionComponent, cuboids));
    }

    template <CollisionAbility ability>
    CollisionResults getFilteredWithAbility() {
        ResultsVector filteredResults;
        for (const auto& result : collisionResults) {
            const auto& collisionComponent = std::get<0>(result);
            const auto& cuboids = std::get<1>(result);
            if(ability == CollisionAbility::WALKABLE) {
                if (collisionComponent->canWalkOn()) {
                    filteredResults.push_back(std::make_tuple(collisionComponent, cuboids));
                }
            }
        }
        return CollisionResults{collisionDetectorCmp, filteredResults.begin(), filteredResults.end()};
    }

private:
    /* The one moving toward collision component */
    CollisionDetectorComponent* collisionDetectorCmp{nullptr};

    /* Related to collision component envountered by detector */
    ResultsVector collisionResults;
};