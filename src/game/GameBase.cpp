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
    Window::addKeyboardListener(this);

    movementSystem.init();

    collisionsSystem.init();
    
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

bool GameBase::onKeyReleased(int key) {
    if(key == GLFW_KEY_I) {
        /* Print some informations */
        auto locationCmp = world.player.getComponent<LocationComponent>();
        if(locationCmp) {
            std::cout << " _________________________________________________" << std::endl;
            std::cout << "| Player location: " << locationCmp->worldRect.top_left << std::endl;
            std::cout << "| Render batch size: " << render_system.getLastEntitesCount() << std::endl;
            std::cout << "| World floor.back() entities count: " << world.floors.back().getTotalEntitiesCount() << std::endl;
            std::cout << std::endl;
        }
        return true;
    }

    return false;
}


bool GameBase::input() {
    // user_input_system.process_keyboard();
    return false;
}

void GameBase::update(float dt) {
    for(auto entity: world.entities) {
        movementSystem.update(entity, dt);
    }


    /* Update append-removal idea:
     * UPDATE AND RENDER HAVE 2 WORLD-VIEW ACTIVE BOXES
     * update box is bigger than render box
     * render box = viewport with slight offset
     * there are additional vectors for pointers to entites in world.
     * tsose "batches" are recycled every loop somehow
     *   t would be best to have some apeends removal based on entering and leaving those boxes
     *   - clearing all every loop is not the best option
    */
        
    collisionsSystem.update(world.entities, &world.player, dt);
    
    camera.update(dt);
    
}

void GameBase::render() {
    render_system.prepare();
    //todo sorting based on z index and Y axis
    for(auto entity: world.entities) {
        render_system.processEntity(entity);
    }
    render_system.render();

    for(auto collisionRect: collisionsSystem.getLastCheckResults()) {
        render_system.renderTranslucentFilledBox(collisionRect, 0.0F, 1.0F, 1.0F, 0.2F);
    }
}
