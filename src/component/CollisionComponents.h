#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include <vector>

class CollisionResult {
public:
    CollisionResult(Entity* collidedEntity, const Cuboid6F& collidingCuboidElevationSpace) : collidedEntity{collidedEntity}, collidingCuboidElevationSpace{collidingCuboidElevationSpace} {}
    operator bool() const {
        return (collidedEntity != nullptr);
    }

    Entity* getCollidedEntity() const {
        return collidedEntity;
    }

    const Cuboid6F& getCollidingCuboidElevationSpace() const {
        return collidingCuboidElevationSpace;
    }

    static CollisionResult NONE() {
        return CollisionResult{nullptr, {}};
    }

private:
    Entity* collidedEntity{nullptr};
    Cuboid6F collidingCuboidElevationSpace;
};

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

    void onCollision(Entity* collidedEntity, const Cuboid6F& collidedCuboidElevationSpace) {};

    CollisionResult checkCollision(CollisionComponent& other) const;

    MovementComponent* movementCmp{nullptr};
    Cuboid6F getElevationSpaceBoundingCuboid() const;
    Cuboid6F getWorldSpaceBoundingCuboid() const;

private:
    Cuboid6F boundingCuboid{0.0F, 0.15F, 0.0F, 1.0F, 0.85F, 1.0F};
};

