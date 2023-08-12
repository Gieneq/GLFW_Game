#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"

enum class Direction {
    NONE,
    NORTH,
    SOUTH,
    WEST,
    EAST
};
class CollisionDetectorComponent;
class MovementComponent : public Component {
public:
    MovementComponent(Entity* e) : Component(e) {}
    virtual ~MovementComponent() = default;

    void update(float dt);
    void setDirection(Direction dir);

    float speed{1.0F};

private:
    Vect2F direction{0.0F, 1.0F};
    Vect2F getTranslation(float dt) const;

    friend class CollisionDetectorComponent;
};
