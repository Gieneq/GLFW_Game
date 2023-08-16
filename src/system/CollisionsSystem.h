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
    void update(
        std::vector<CollisionComponent*>::iterator collisionCmpsBegin,
        std::vector<CollisionComponent*>::iterator collisionCmpsEnd,
        Entity *entity, float dt);

    inline std::vector<Rect2F>::iterator getCollidingRectsBegin() {
        return collidingRects.begin();
    }

    inline std::vector<Rect2F>::iterator getCollidingRectsEnd() {
        return collidingRects.end();
    }

    inline std::vector<Rect2F>::const_iterator getCollidingRectsBegin() const {
        return collidingRects.begin();
    }

    inline std::vector<Rect2F>::const_iterator getCollidingRectsEnd() const {
        return collidingRects.end();
    }

    int getCollidingRectsCount() const {
        return static_cast<int>(collidingRects.size());
    }


private:
    std::vector<Rect2F> collidingRects;
};

