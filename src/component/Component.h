#pragma once
#include <iostream>

class Entity;
class Component {
public:
    virtual ~Component() = default;
    Component(Entity* parent) : parent{parent} {}
    virtual Component* clone(Entity* new_parent) = 0;
    
protected:
    Entity* parent{nullptr};
};
