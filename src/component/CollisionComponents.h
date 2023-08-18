#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include <vector>

class CollisionResult {
public:
    CollisionResult(Entity* collidedEntity, const std::vector<Cuboid6F>& collidingCuboidsElevationSpace) : collidedEntity{collidedEntity}, collidingCuboidsElevationSpace{collidingCuboidsElevationSpace} {}
    operator bool() const {
        return collidingCuboidsElevationSpace.size() > 0;
    }

    Entity* getCollidedEntity() const {
        return collidedEntity;
    }

    std::vector<Cuboid6F>::iterator begin() {
        return collidingCuboidsElevationSpace.begin();
    }

    std::vector<Cuboid6F>::iterator end() {
        return collidingCuboidsElevationSpace.end();
    }

    std::vector<Cuboid6F>::const_iterator begin() const {
        return collidingCuboidsElevationSpace.begin();
    }

    std::vector<Cuboid6F>::const_iterator end() const {
        return collidingCuboidsElevationSpace.end();
    }

    Cuboid6F& operator[](int index) {
        return collidingCuboidsElevationSpace[index];
    }

    const Cuboid6F& operator[](int index) const {
        return collidingCuboidsElevationSpace[index];
    }

    static CollisionResult NONE() {
        return CollisionResult{nullptr, {}};
    }

private:
    Entity* collidedEntity{nullptr};

    std::vector<Cuboid6F> collidingCuboidsElevationSpace;
};

class Loader;
class CollisionComponent : public Component {
public:
    CollisionComponent(Entity* e) : Component(e) {}
    virtual ~CollisionComponent() = default;


    std::vector<Cuboid6F> getElevationSpaceCollisionCuboids() const;
    std::vector<Cuboid6F> getWorldSpaceCollisionCuboids() const;

private:
    inline void appendCollisionCuboid(const Cuboid6F& cuboid) {
        collisionCuboids.push_back(cuboid);
    }
    
    /* Local spaced */
    std::vector<Cuboid6F> collisionCuboids;
    friend class Loader;
};


class MovementComponent;
class CollisionDetectorComponent : public Component {
public:
    CollisionDetectorComponent(Entity* e, MovementComponent* movmnt, const Cuboid6F& bnd) : Component(e), movementCmp{movmnt}, boundingCuboid{bnd} {}
    virtual ~CollisionDetectorComponent() = default;

    void onCollision(Entity* collidedEntity, std::vector<Cuboid6F>::const_iterator elevationSpaceCuboidsBegin, std::vector<Cuboid6F>::const_iterator elevationSpaceCuboidsEnd) {};

    CollisionResult checkCollision(CollisionComponent& other) const;

    MovementComponent* movementCmp{nullptr};
    Cuboid6F getElevationSpaceBoundingCuboid() const;
    Cuboid6F getWorldSpaceBoundingCuboid() const;

private:
    Cuboid6F boundingCuboid{0.0F, 0.15F, 0.0F, 1.0F, 0.85F, 1.0F};
};

