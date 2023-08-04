#pragma once
#include <vector>
#include "Camera.h"
#include "Core.h"
#include "RenderSystem.h"
#include "UserInputSystem.h"
#include "TransformSystem.h"


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
