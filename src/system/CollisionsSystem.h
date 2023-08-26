#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include <vector>
#include "Maths.h"
#include "CollisionResult.h"

class Entity;
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
    void onCollision();
    bool reactToCollision(CollisionDetectorComponent* detector , const Cuboid6F& collidedCuboidElevationSpace);
    void attemptAlignToEdge(CollisionDetectorComponent* detector , const Cuboid6F& collidedCuboidElevationSpace);
    // void CollisionsSystem::stopMovingComponent(MovementComponent* movementCmp,
    //     const Cuboid6F& boundingCuboidElevationSpace, const Cuboid6F& collidedCuboidElevationSpace);

public:    
    std::vector<Cuboid6F> getCollisionResultsCuboidsWorldSpace();

    inline int getCollisionResultsCount() const {
        return static_cast<int>(collisionResults.size());
    }

private:
/* */
    std::vector<CollisionResult> collisionResults;
};

