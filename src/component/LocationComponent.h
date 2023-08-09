#pragma once
#include "Component.h"
#include "Maths.h"

// class TransparentComponent : public Component

class LocationComponent : public Component {
public:
    LocationComponent(Entity* e) : Component(e) {}
    virtual ~LocationComponent() = default;
    Rect2F worldRect{0.0F,0.0F,1.0F,1.0F};
    float zIndex{0.0F};

    LocationComponent* clone(Entity* new_parent) override {
        return new LocationComponent(new_parent);
    }
};
