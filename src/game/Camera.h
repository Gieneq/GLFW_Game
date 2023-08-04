#pragma once
#include "Settings.h"

class Entity;
class TransformComponent;
class Camera {
public:
    Vect2F position{0.0F, 0.0F};
    Size2F zoom{Settings::Camera::ZOOM, Settings::Camera::ZOOM};
    void update(float dt);
    void focus_on(Entity *e);

    Entity * focused_entity;
    TransformComponent * focused_component;
};