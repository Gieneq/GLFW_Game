#pragma once
#include <iostream>
#include <vector>

#include "SystemBase.h"
#include "../core/Settings.h"
#include "../entity/Entity.h"

class TransformSystem : public SystemBase{
public:
    int system_id;
    void init();
    void update(Entity *entity, float dt);
};

