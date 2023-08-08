#pragma once
#include <iostream>
#include <vector>

#include "SystemBase.h"
#include "Settings.h"
#include "Entity.h"
#include <vector>

class CollisionsSystem : public SystemBase {
public:
    int system_id;
    void init();
    void update(const std::vector<Entity*>& entities, Entity *entity, float dt);

    std::vector<Rect2F>& getLastCheckResults() {
        return collidingRects;
    }
private:
    std::vector<Rect2F> collidingRects;
};

