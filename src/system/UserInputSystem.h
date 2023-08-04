#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include "Callbacks.h"
// #include "ControllableComponent.h"
#include "Entity.h"

class UserInputSystem : public SystemBase, public KeyboardEvent {
public:
    int system_id;
    void init();
    void attach_controllabe(Entity *entity);
    virtual bool on_key_press(int key);
    virtual bool on_key_release(int key);
private:
    Entity *controllable_entity{nullptr};
    // WSADControllableComponent * wsad_controllable{nullptr};
};

