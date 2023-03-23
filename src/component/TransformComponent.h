#pragma once
#include <iostream>
#include "Component.h"
#include "../utils/Maths.h"

class TransformComponent : public Component {
public:
    virtual ~TransformComponent() = default;
    TransformComponent(Entity* e, Vect2F& target) : Component(e), target{target} {}

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
};
