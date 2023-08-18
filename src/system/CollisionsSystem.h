#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include <vector>
#include "Maths.h"

class Entity;
class CollisionComponent;
class CollisionDetectorComponent;
class MovementComponent;
class CollisionsSystem : public SystemBase {
public:
    int system_id;
    void init();
    void update(
        std::vector<CollisionComponent*>::iterator collisionCmpsBegin,
        std::vector<CollisionComponent*>::iterator collisionCmpsEnd,
        Entity *entity, float dt);

private:
    void onCollision(CollisionDetectorComponent* collisionDetectorCmp, 
        CollisionComponent* collidedComponent, const Cuboid6F& collidedCuboidElevationSpace);

    void CollisionsSystem::stopMovingComponent(MovementComponent* movementCmp,
        const Cuboid6F& boundingCuboidElevationSpace, const Cuboid6F& collidedCuboidElevationSpace);

public:    
    inline std::vector<Cuboid6F>::iterator getCollidingCuboidsWorldSpaceBegin() {
        return collidingCuboidsWorldSpace.begin();
    }

    inline std::vector<Cuboid6F>::iterator getCollidingCuboidsWorldSpaceEnd() {
        return collidingCuboidsWorldSpace.end();
    }

    inline std::vector<Cuboid6F>::const_iterator getCollidingCuboidsWorldSpaceBegin() const {
        return collidingCuboidsWorldSpace.begin();
    }

    inline std::vector<Cuboid6F>::const_iterator getCollidingCuboidsWorldSpaceEnd() const {
        return collidingCuboidsWorldSpace.end();
    }

    int getCollidingCuboidsCount() const {
        return static_cast<int>(collidingCuboidsWorldSpace.size());
    }

private:
/* */
    std::vector<Cuboid6F> collidingCuboidsWorldSpace;
};

