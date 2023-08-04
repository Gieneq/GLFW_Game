#include "GameBase.h"
#include <iostream>
#include "Window.h"
#include "Callbacks.h"
#include "Loader.h"
#include "IO.h"
#include "World.h"
#include "Component.h"
#include "FoodComponent.h"
#include "TransformComponent.h"
#include "Entity.h"

void GameBase::init() {
    Core::init();
    render_system.init();
    render_system.attachCamera(&camera);

    user_input_system.init();
    Window::addKeyboardListener(&user_input_system);

    transform_system.init();
}


bool GameBase::input() {
    // user_input_system.process_keyboard();
    return false;
}

void GameBase::update(float dt) {
    for(auto entity: entities) {
        transform_system.update(entity, dt);
    }
    
    camera.update(dt);
}

void GameBase::render() {
    //todo sorting based on z index and Y axis
    for(auto entity: entities) {
        render_system.render(entity);
    }
}
