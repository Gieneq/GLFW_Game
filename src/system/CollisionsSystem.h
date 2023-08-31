#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include "SystemBase.h"
#include "Settings.h"
#include "Maths.h"
#include "CollisionResult.h"

class Entity;
class MovementComponent;
class CollisionComponent;
class CollisionDetectorComponent;
class TilesQuad;


class CollisionsSystem : public SystemBase {
public:
    // using EntitiesList = std::vector<Entity*>;
    // using ObstaclesList = std::vector<CollisionComponent*>;

    int system_id;
    void init();
    void processDetector(Entity* detectorEntity);
    std::vector<Cuboid6F> getDebugResultsCuboidsWorldSpace();

    /* Frontal means colliding to front edge */
    // static ObstaclesList detectorGetFrontalObstacles(Entity* detectorEntity);
    // static ObstaclesList detectorGetFrontalObstacles(Entity* detectorEntity, int elevationIndex);

    // static std::optional<CollisionComponent*> getHighestObstacle(const ObstaclesList& collidingComponents);
    // static ObstaclesList retriveObstaclessFromEntities(const EntitiesList& entities);
    // static EntitiesList retriveEntitiesFromObstacles(const ObstaclesList& collidingComponents);
    // static std::vector<Cuboid6F> retriveElevationSpaceCuboidsFromObstacles(const ObstaclesList& collidingComponents);
    // static std::vector<Cuboid6F> retriveWorldSpaceCuboidsFromObstacles(const ObstaclesList& collidingComponents);



private:
    void onCollisionAlignToEdge(Cuboid6F& detectorElevationSpace, const Cuboid6F boundignBoxElevationSpace, const Cuboid6F& obstacleElevationSpace, Vect2F direction);
    void onCollisionStandOntop(Cuboid6F& detectorElevationSpace, const Cuboid6F& obstacleElevationSpace);
    void alignEntityCuboidToTilesQuad(Entity* entity, TilesQuad* tilesQuad, Vect2F direction);

    std::vector<Entity*> debugEntites;
};

