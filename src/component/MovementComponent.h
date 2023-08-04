#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"

//enum of 4 types of direction
enum class Direction {
    NORTH,
    SOUTH,
    WEST,
    EAST
};

class LocationComponent;
class MovementComponent : public Component {
public:
    MovementComponent(Entity* e, LocationComponent* loc) : Component(e), parentLocation{loc} {}
    virtual ~MovementComponent() = default;

    void update(float dt);
    void setDirection(Direction dir);
    Vect2F getTranslation(float dt) const;

    LocationComponent* parentLocation{nullptr};
    float speed{1.0F};
    Vect2F direction{0.0F, 1.0F};

    MovementComponent* clone(Entity* new_parent) override {
        return new MovementComponent(new_parent, parentLocation);
    }
};
