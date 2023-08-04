#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include "SpriteComponent.h"
#include "Entity.h"
#include "Camera.h"
#include "Loader.h"

class RenderSystem : public SystemBase {
public:
    int system_id;
    void init();
    void render(Entity* entity);
    void setViewportDimensions(int width, int height);
    void attachCamera(Camera *cam);

private:
    Camera *camera;
    int viewport_width{Settings::Window::WIDTH};
    int viewport_height{Settings::Window::HEIGHT};
    float aspect_ratio{Settings::Window::ASPECT_RATIO};
};

