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

bool GameBase::init() {
    bool result = Core::init();
    if(!result) {
        std::cerr << "Core::init() failed" << std::endl;
        return false;
    }

    /* Setup systems */
    render_system.init();
    render_system.attachCamera(&camera);

    user_input_system.init();
    Window::addKeyboardListener(&user_input_system);

    transform_system.init();
    
    /* Loader loading */
    result = Loader::getLoader().loadAssets();
    if(!result) {
        std::cerr << "Loader::loadAssets() failed" << std::endl;
        return false;
    }

    result = Loader::getLoader().loadWorld(world);
    if(!result) {
        std::cerr << "Loader::loadWorld() failed" << std::endl;
        return false;
    }

    result = Loader::getLoader().loadPlayer(world);
    if(!result) {
        std::cerr << "Loader::loadPlayer() failed" << std::endl;
        return false;
    }

    /* Set camera on just loaded player */
    camera.focus_on(&world.player);

    /* Attach controller to just loaded player */
    user_input_system.attach_controllabe(&world.player);
    
    return result;
}


bool GameBase::input() {
    // user_input_system.process_keyboard();
    return false;
}

void GameBase::update(float dt) {
    for(auto entity: world.entities) {
        transform_system.update(entity, dt);
    }
    
    camera.update(dt);
}

void GameBase::render() {
    //todo sorting based on z index and Y axis
    for(auto entity: world.entities) {
        render_system.render(entity);
    }
}
