#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"

class TransformComponent : public Component {
public:
    TransformComponent(Entity* e, Vect2F& target) : Component(e), target{target} {}
    virtual ~TransformComponent() = default;

    void update(float dt);
    void direction_north();
    void direction_south();
    void direction_west();
    void direction_east();
    void go();
    void stop();

    float speed{1.0F};
    Vect2F direction{0.0F, 1.0F};
    bool moving{false};
    Vect2F& target;

    TransformComponent* clone(Entity* new_parent) override {
        return new TransformComponent(new_parent, target);
    }
};
