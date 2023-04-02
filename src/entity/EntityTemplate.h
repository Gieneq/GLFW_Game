#pragma once
#include "EntityGlobalId.h"
#include "Entity.h"
#include <string>

class EntityTemplate {
public:
    std::string name{};
    EntityGlobalId id{};
    Entity* entity {nullptr};

    Entity* create_new() {
        return entity->clone();
    }
};

