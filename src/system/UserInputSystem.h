#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include "Callbacks.h"

class ControllableComponent;
class Entity;
class UserInputSystem : public SystemBase, public KeyboardEvent {
public:
    int system_id;
    void init();
    void attachControllabe(Entity *entity);
    virtual bool onKeyPressed(int key);
    virtual bool onKeyReleased(int key);
private:
    ControllableComponent * controllableObject{nullptr};
};

