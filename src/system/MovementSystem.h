#pragma once
#include <iostream>
#include <vector>

#include "SystemBase.h"
#include "Settings.h"
#include "Entity.h"

class MovementSystem : public SystemBase{
public:
    int system_id;
    void init();
    void update(Entity *entity, float dt);
};

