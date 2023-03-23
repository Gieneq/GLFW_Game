#pragma once
#include <iostream>
#include <vector>

#include "SystemBase.h"
#include "../core/Settings.h"
#include "../core/Callbacks.h"
#include "../component/ControllableComponent.h"
#include "../entity/Entity.h"

class UserInputSystem : public SystemBase, public KeyboardEvent {
public:
    int system_id;
    void init();
    void attach_controllabe(Entity *entity);
    virtual bool on_key_press(int key);
    virtual bool on_key_release(int key);
private:
    Entity *controllable_entity{nullptr};
    WSADControllableComponent * wsad_controllable{nullptr};
};

