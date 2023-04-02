#pragma once
#include <iostream>
#include <vector>



#include "SystemBase.h"
#include "../core/Settings.h"
#include "../component/SpriteComponent.h"
#include "../entity/Entity.h"
#include "../game/Camera.h"
#include "Loader.h"

class RenderSystem : public SystemBase {
public:
    int system_id;
    void init();
    void render(Entity* entity);
    void set_viewport_size(int width, int height);
    void attach_camera(Camera *cam);
private:
    Camera *camera;
    int viewport_width{Settings::Window::WIDTH};
    int viewport_height{Settings::Window::HEIGHT};
    float aspect_ratio{Settings::Window::ASPECT_RATIO};
};

