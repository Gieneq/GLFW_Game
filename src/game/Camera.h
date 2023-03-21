#pragma once
#include "../core/Settings.h"
#include "../utils/Maths.h"

class Camera {
public:
    Vect2F position{0.0F, 0.0F};
    Size2F zoom{Settings::Camera::ZOOM, Settings::Camera::ZOOM};
};