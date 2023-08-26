#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include <vector>

class CollisionDetectorComponent;
class CollisionComponent;

class CollisionResult {
public:
    CollisionResult(CollisionDetectorComponent* collisionDetectorCmp, CollisionComponent* collidedComponent) : 
        collisionDetectorCmp{collisionDetectorCmp}, collidedComponent{collidedComponent} {}

    operator bool() const {
        return size() > 0;
    }

    CollisionComponent* getCollidedComponent() const {
        return collidedComponent;
    }

    CollisionDetectorComponent* getCollisionDetectorComponent() const {
        return collisionDetectorCmp;
    }

    std::vector<Cuboid6F>::iterator begin() {
        return collidedCuboidsElevationSpace.begin();
    }

    std::vector<Cuboid6F>::iterator end() {
        return collidedCuboidsElevationSpace.end();
    }

    std::vector<Cuboid6F>::const_iterator begin() const {
        return collidedCuboidsElevationSpace.begin();
    }

    std::vector<Cuboid6F>::const_iterator end() const {
        return collidedCuboidsElevationSpace.end();
    }

    size_t size() const {
        return collidedCuboidsElevationSpace.size();
    }

    inline int getCollidedCuboidsCount() const {
        return static_cast<int>(collidedCuboidsElevationSpace.size());
    }

    static CollisionResult NONE() {
        return CollisionResult{nullptr, nullptr};
    }

    void addCollidedCuboid(const Cuboid6F& cuboid) {
        collidedCuboidsElevationSpace.push_back(cuboid);
    }

private:
    /* The one moving toward collision component */
    CollisionDetectorComponent* collisionDetectorCmp{nullptr};

    /* Related to collision component envountered by detector */
    CollisionComponent* collidedComponent{nullptr};
    std::vector<Cuboid6F> collidedCuboidsElevationSpace;
};