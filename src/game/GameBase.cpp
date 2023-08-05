#include "GameBase.h"
#include <iostream>
#include "Window.h"
#include "Callbacks.h"
#include "Loader.h"
#include "IO.h"
#include "World.h"
#include "Component.h"
#include "ControllableComponent.h"
#include "LocationComponent.h"

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

    userInputSystem.init();
    Window::addKeyboardListener(&userInputSystem);

    movementSystem.init();
    
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
    camera.focusOn(world.player.getComponent<LocationComponent>());

    /* Attach controller to just loaded player */
    userInputSystem.attachControllabe(&world.player);
    
    return result;
}


bool GameBase::input() {
    // user_input_system.process_keyboard();
    return false;
}

void GameBase::update(float dt) {
    for(auto entity: world.entities) {
        movementSystem.update(entity, dt);
    }
    
    camera.update(dt);
}

void GameBase::render() {
    //todo sorting based on z index and Y axis
    for(auto entity: world.entities) {
        render_system.render(entity);
    }
}
