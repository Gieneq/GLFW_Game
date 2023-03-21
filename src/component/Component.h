#pragma once
#include <iostream>

class Entity;
class Component {
public:
    virtual ~Component() = default;
    Component(Entity*parent) : parent{parent} {}
    Entity* parent; 
};
