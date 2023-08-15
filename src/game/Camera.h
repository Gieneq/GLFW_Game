#pragma once
#include "Settings.h"
#include "Maths.h"

class Entity;
class Camera {
public:
    Vect3F position{0.0F, 0.0F, 0.0F};
    Size2F zoom{Settings::Camera::ZOOM, Settings::Camera::ZOOM};
    void update(float dt);
    void focusOn(Entity* e);

    inline Entity* getFocusedEntity() {
        return focusedEntity;
    }
    Entity* focusedEntity{nullptr};
};