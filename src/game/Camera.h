#pragma once
#include "Settings.h"

class Entity;
class LocationComponent;
class Camera {
public:
    Vect2F position{0.0F, 0.0F};
    Size2F zoom{Settings::Camera::ZOOM, Settings::Camera::ZOOM};
    void update(float dt);
    void focusOn(LocationComponent *loc);

    LocationComponent * focusedLocation;
};