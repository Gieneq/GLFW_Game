#pragma once
#include <iostream>

class Entity;
class Component {
public:
    virtual ~Component() = default;
    Component(Entity* parent) : parent{parent} {}

    inline Entity* getParentEntity() {
        return parent;
    }
    
protected:
    Entity* parent{nullptr};
};
