#pragma once
#include <vector>

#include "Camera.h"
#include "../core/Core.h"
#include "../entity/Entity.h"

#include "../system/RenderSystem.h"
#include "../system/UserInputSystem.h"
#include "../system/TransformSystem.h"


class GameBase : public Core {
public:
    virtual ~GameBase() = default;

    virtual void init() override;
    virtual bool input() override;
    virtual void update(float dt) override;
    virtual void render() override;

protected:
    Camera camera;
    std::vector<Entity*> entities;
    
    RenderSystem render_system;
    UserInputSystem user_input_system;
    TransformSystem transform_system;
};
