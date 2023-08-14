#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"

class CollisionDetectorComponent;
class MovementComponent : public Component {
public:
    MovementComponent(Entity* e) : Component(e) {}
    virtual ~MovementComponent() = default;

    void update(float dt);
    void setDirection(float x, float y, float z);
    void setDirectionUnsafe(float x, float y, float z);

    float speed{1.0F};

private:
    Vect3F direction{0.0F, 1.0F, 0.0F};
    Vect3F getTranslation(float dt) const;

    friend class CollisionDetectorComponent;
};
