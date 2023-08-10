#pragma once
#include <vector>
#include "Camera.h"
#include "Core.h"
#include "RenderSystem.h"
#include "UserInputSystem.h"
#include "MovementSystem.h"
#include "CollisionsSystem.h"
#include "World.h"
#include "Entity.h"
#include "Callbacks.h"

class GameBase : public Core, public KeyboardEventListener {
public:
    virtual ~GameBase() = default;

    virtual bool init() override;
    virtual bool input() override;
    virtual void update(float dt) override;
    virtual void render() override;
    virtual bool onKeyPressed(int key) { 
        return false;
    }
    virtual bool onKeyReleased(int key);

protected:
    Camera camera;
    World world;
    RenderSystem render_system;
    UserInputSystem userInputSystem;
    MovementSystem movementSystem;
    CollisionsSystem collisionsSystem;
    bool debugView{false};
};
