#pragma once
#include "../core/Settings.h"
#include "../utils/Maths.h"
#include "../entity/Entity.h"
#include "../component/TransformComponent.h"

class Camera {
public:
    Vect2F position{0.0F, 0.0F};
    Size2F zoom{Settings::Camera::ZOOM, Settings::Camera::ZOOM};
    void update(float dt);
    void focus_on(Entity *e);

    Entity * focused_entity;
    TransformComponent * focused_component;
};