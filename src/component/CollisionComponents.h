#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include "Coordinates.h"
#include <vector>
#include "CollisionResult.h"

class CollisionDetectorComponent;
class Loader;
class CollisionComponent : public Component {
public:
    CollisionComponent(Entity* e) : Component(e) {}
    virtual ~CollisionComponent() = default;

    std::vector<ElevationCuboid> getElevationCollisionCuboids() const;
    std::vector<WorldCuboid> getWorldCollisionCuboids() const;

    inline bool canWalkOn() const {
        return allowWalkOn;
    }

    static std::vector<ElevationCuboid> retriveElevationCuboidsFromComponents(const std::vector<CollisionComponent*>::const_iterator& begin, const std::vector<CollisionComponent*>::const_iterator& end);

private:
    inline void appendCollisionCuboid(const Cuboid6F& cuboid) {
        collisionCuboids.push_back(cuboid);
    }
    
    /* Local spaced */
    std::vector<Cuboid6F> collisionCuboids;
    bool allowWalkOn{false};
    friend class Loader;
};


class MovementComponent;
class CollisionDetectorComponent : public Component {
public:
    using CollisonCmpsVector = std::vector<CollisionComponent*>;
    CollisionDetectorComponent(Entity* e, MovementComponent* movmnt, const Cuboid6F& bnd) : Component(e), movementCmp{movmnt}, boundingCuboid{bnd} {}
    virtual ~CollisionDetectorComponent() = default;

    void onCollision(const CollisionResults& results) {};

    CollisionResults checkCollision(const CollisonCmpsVector::const_iterator& begin, const CollisonCmpsVector::const_iterator& end) const;

    MovementComponent* movementCmp{nullptr};
    ElevationCuboid getElevationBoundingCuboid() const;

private:
    Cuboid6F boundingCuboid{0.0F, 0.15F, 0.0F, 1.0F, 0.85F, 1.0F};
};

