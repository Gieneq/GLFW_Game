#pragma once
#include <iostream>
#include "Component.h"
#include "../utils/Maths.h"

class ColorComponent : public Component {
public:
    virtual ~ColorComponent() = default;
    ColorComponent(Entity* e) : Component(e) {}
    float r{0.56f};
    float g{1.0f};
    float b{1.0f};
    Rect2F rect{0.0F,0.0F,1.0F,1.0F};
};

class SpriteComponent : public Component {
public:
    virtual ~SpriteComponent() = default;

};
