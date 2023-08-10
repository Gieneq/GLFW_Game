#pragma once
#include <iostream>
#include <optional>

class Entity;
class Component {
public:
    virtual ~Component() = default;
    Component(Entity* parent) : parent{parent} {}
    virtual Component* clone(Entity* new_parent) = 0;

    std::optional<Entity*> getParent() const {
        return parent;
    }
    
protected:
    Entity* parent{nullptr};
};
