#pragma once
#include <vector>
#include "Entity.h"

class Floor {
public:
    std::vector<Entity*> floorEntities;
    std::vector<Entity*> clutterEntities;
    std::vector<Entity*> staticEntities;
    std::vector<Entity*> dynamicEntities;

    int getTotalEntitiesCount() const {
        return floorEntities.size() + clutterEntities.size() + staticEntities.size() + dynamicEntities.size();
    }
};

class World {
public:
    // std::vector<Entity*> entities;
    std::vector<Floor> floors;
    Entity player;
};
