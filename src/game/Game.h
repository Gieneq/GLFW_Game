#pragma once
#include <vector>
#include <iostream>

#include "Camera.h"
#include "../core/Core.h"
#include "../entity/Entity.h"

#include "../component/Component.h"
#include "../component/FoodComponent.h"
#include "../component/TransformComponent.h"

#include "../system/RenderSystem.h"
#include "../system/UserInputSystem.h"
#include "../system/TransformSystem.h"


class Game : public Core {
public:
    virtual ~Game() = default;

    void init();
    virtual bool input() override;
    virtual void update(float dt) override;
    virtual void render() override;

    Camera camera;
    std::vector<Entity*> entities;
    
    RenderSystem render_system;
    UserInputSystem user_input_system;
    TransformSystem transform_system;
};
