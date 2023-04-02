#pragma once
#include <iostream>
#include "Component.h"

class FoodComponent : public Component {
public:
    FoodComponent(Entity* e) : Component(e) {}
    virtual ~FoodComponent() = default;

    void eat();

    FoodComponent* clone(Entity* new_parent) override {
        return new FoodComponent(new_parent);
    }
};
