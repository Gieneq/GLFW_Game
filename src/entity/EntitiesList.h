#pragma once

#include <vector>
#include <optional>
#include <iostream>

#include "Entity.h"
#include "Maths.h"

class CollisionComponent;
class EntitiesList {
public:
    EntitiesList(const std::vector<Entity*>& entities) : entities{entities} {}
    EntitiesList() = default;

    void operator+=(Entity* entity) {
        add(entity);
    }
    Entity* operator[](int index) {
        return entities[index];
    }

    void operator-= (Entity* entity) {
        remove(entity);
    }

    inline void add(Entity* entity) {
        entities.push_back(entity);
    }

    void remove(Entity* entity);

    inline const std::vector<Entity*>& get() const {
        return entities;
    }

    inline std::vector<Entity*>& get() {
        return entities;
    }

    std::vector<CollisionComponent> getCollisionComponents() const;

    Cuboid6F getBoundingBoxElevationSpace() const;


private:
    std::vector<Entity*> entities;
};