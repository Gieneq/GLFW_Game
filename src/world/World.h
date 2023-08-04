#pragma once
#include <vector>
#include "Entity.h"

class World {
public:
    std::vector<Entity*> entities;
    Entity player;
};
