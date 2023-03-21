#pragma once
#include <iostream>
#include "Component.h"

class ColorComponent : public Component {
public:
    virtual ~ColorComponent() = default;
    ColorComponent(Entity* e) : Component(e) {}
    float r{0.56f};
    float g{1.0f};
    float b{1.0f};
};

class SpriteComponent : public Component {
public:
    virtual ~SpriteComponent() = default;

};
