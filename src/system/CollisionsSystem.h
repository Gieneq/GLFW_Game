#pragma once
#include <iostream>
#include <vector>
#include <optional>
#include "SystemBase.h"
#include "Settings.h"
#include "Maths.h"
#include "CollisionResult.h"
#include "Coordinates.h"

class Entity;
class Elevation;
class MovementComponent;
class CollisionComponent;
class CollisionDetectorComponent;

class CollisionsSystem : public SystemBase {
public:
    using EntitiesList = std::vector<Entity*>;
    using CollisionComponentsList = std::vector<CollisionComponent*>;
    using ElevationCuboidsList = std::vector<ElevationCuboid>;
    using InitialRequirements = std::tuple<CollisionDetectorComponent*, MovementComponent*, Elevation*>;

    int system_id;
    void init();
    void startSession();
    void processDetector(Entity* detectorEntity, const CollisionComponentsList::const_iterator& collisionComponentsBegin, 
        const CollisionComponentsList::const_iterator& collisionComponentsEnd);
    std::vector<WorldCuboid> getDebugResults();
    std::vector<WorldCuboid> getWalkableObstaclesCuboids();
    std::vector<WorldCuboid> getNotWalkableObstaclesCuboids();

private:
    static std::optional<InitialRequirements> checkDetectororRequirements(Entity* detectorEntity);

    static void leanAgainstObstacle(CollisionDetectorComponent* detectorCmp, const Vect2F& direction, const ElevationCuboid& obstacle);

    static void standOnObstacle(CollisionDetectorComponent* detectorCmp, const ElevationCuboid& obstacle);
    

    std::vector<CollisionResults> lastResults;
    std::vector<Entity*> debugEntites;
    std::vector<ElevationCuboid> walkableCuboids;
    std::vector<ElevationCuboid> notWalkableCuboids;
};

