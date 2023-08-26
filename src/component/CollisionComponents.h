#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include <vector>
#include "CollisionResult.h"

class CollisionDetectorComponent;
class Loader;
class CollisionComponent : public Component {
public:
    CollisionComponent(Entity* e) : Component(e) {}
    virtual ~CollisionComponent() = default;


    std::vector<Cuboid6F> getElevationSpaceCollisionCuboids() const;
    std::vector<Cuboid6F> getWorldSpaceCollisionCuboids() const;

    inline bool canWalkOn() const {
        return allowWalkOn;
    }

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
    CollisionDetectorComponent(Entity* e, MovementComponent* movmnt, const Cuboid6F& bnd) : Component(e), movementCmp{movmnt}, boundingCuboid{bnd} {}
    virtual ~CollisionDetectorComponent() = default;

    void onCollision(const CollisionResult& result) {};

    CollisionResult checkCollision(CollisionComponent& other);

    MovementComponent* movementCmp{nullptr};
    Cuboid6F getElevationSpaceBoundingCuboid() const;
    Cuboid6F getWorldSpaceBoundingCuboid() const;

private:
    Cuboid6F boundingCuboid{0.0F, 0.15F, 0.0F, 1.0F, 0.85F, 1.0F};
};

