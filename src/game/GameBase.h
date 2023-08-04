#pragma once
#include <vector>
#include "Camera.h"
#include "Core.h"
#include "RenderSystem.h"
#include "UserInputSystem.h"
#include "MovementSystem.h"
#include "World.h"
#include "Entity.h"

class GameBase : public Core {
public:
    virtual ~GameBase() = default;

    virtual bool init() override;
    virtual bool input() override;
    virtual void update(float dt) override;
    virtual void render() override;

protected:
    Camera camera;
    World world;
    RenderSystem render_system;
    UserInputSystem user_input_system;
    MovementSystem movementSystem;
};
