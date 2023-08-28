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
class CollisionComponent;
class CollisionDetectorComponent;
class TilesQuad;


class CollisionsSystem : public SystemBase {
public:
    int system_id;
    void init();
    void processDetector(Entity* detectorEntity);
    // void update(
    //     std::vector<CollisionComponent*>::iterator collisionCmpsBegin,
    //     std::vector<CollisionComponent*>::iterator collisionCmpsEnd,
    //     Entity *entity, float dt);

private:
    // void onCollision();
    // bool reactToCollision(CollisionDetectorComponent* detector , const Cuboid6F& collidedCuboidElevationSpace);
    // void attemptAlignFaceOpposite(CollisionDetectorComponent* detector , const Cuboid6F& collidedCuboidElevationSpace);
    // void CollisionsSystem::stopMovingComponent(MovementComponent* movementCmp,
    //     const Cuboid6F& boundingCuboidElevationSpace, const Cuboid6F& collidedCuboidElevationSpace);

public:    
    std::vector<Cuboid6F> getDebugResultsCuboidsWorldSpace();

    // inline int getCollisionResultsCount() const {
    //     return static_cast<int>(collisionResults.size());
    // }

private:
    void onCollisionAlignToEdge(Cuboid6F& detectorElevationSpace, const Cuboid6F boundignBoxElevationSpace, const Cuboid6F& obstacleElevationSpace, Vect2F direction);
    void onCollisionStandOntop(Cuboid6F& detectorElevationSpace, const Cuboid6F& obstacleElevationSpace);
    void alignEntityCuboidToTilesQuad(Entity* entity, TilesQuad* tilesQuad, Vect2F direction);
/* */
    std::vector<Entity*> debugEntites;
};

