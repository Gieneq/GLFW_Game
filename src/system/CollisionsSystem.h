#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include <vector>
#include "Maths.h"

class Entity;
class CollisionComponent;
class CollisionsSystem : public SystemBase {
public:
    int system_id;
    void init();
    void update(const std::vector<CollisionComponent*>& collisionCmps, Entity *entity, float dt);

    std::vector<Rect2F>& getLastCheckResults() {
        return collidingRects;
    }
private:
    std::vector<Rect2F> collidingRects;
};

