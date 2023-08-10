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
#include "Player.h"

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
        auto player = &world.player;
        auto containingFloor = player->getFloor().value();

        std::cout << " __________________________________________________________" << std::endl;
        std::cout << "| Player location: " << player->locationComponent->worldRect.top_left << "H: " << player->locationComponent->getWorldZ() << "/" << containingFloor->getZ() << std::endl;
        std::cout << "| Render batch size: " << render_system.getLastEntitesCount() << std::endl;
        std::cout << "| Floor [" << containingFloor->elevation << "/" << world.floors.size() << "] info:" << std::endl;
        std::cout << "|  - entities floor:   " << containingFloor->getFloorEntities().size() << std::endl;
        std::cout << "|  - entities clutter: " << containingFloor->getClutterEntities().size() << std::endl;
        std::cout << "|  - entities static:  " << containingFloor->getStaticEntities().size() << std::endl;
        std::cout << "|  - entities dynamic: " << containingFloor->getDynamicEntities().size() << std::endl;
        std::cout << "|  - entities bigger:  " << containingFloor->getBiggerEntities().size() << std::endl;
        std::cout << "|  - components location: " << containingFloor->getLocationComponents().size() << std::endl;
        std::cout << "|  - components movement: " << containingFloor->getMovementComponents().size() << std::endl;
        std::cout << "|  - components collision: " << containingFloor->getCollisionComponents().size() << std::endl;
        std::cout << std::endl;
        return true;
    }

    return false;
}


bool GameBase::input() {
    // user_input_system.process_keyboard();
    return false;
}

void GameBase::update(float dt) {
    auto containingFloorOption = world.player.getFloor();
    if(!containingFloorOption.has_value()) {
        std::cerr << "Player is not on floor!" << std::endl;
        return;
    }
    auto containingFloor = containingFloorOption.value();

    /**
     * Update movement is for dynamicEntities only.
     * Player is also dynamicEntity.
     */
    for(auto dynamicEntity : containingFloor->getDynamicEntities()) {
        movementSystem.update(dynamicEntity, dt);
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
        
    collisionsSystem.update(containingFloor->getCollisionComponents(), &world.player, dt);
    
    camera.update(dt);
    
}

void GameBase::render() {
    auto containingFloorOption = world.player.getFloor();
    if(!containingFloorOption.has_value()) {
        std::cerr << "Player is not on floor!" << std::endl;
        return;
    }
    auto containingFloor = containingFloorOption.value();
    

    /* Render from bottom of containingFloor */

    /* Floor flat tiles */
    render_system.prepare();
    for(auto entity: containingFloor->getFloorEntities()) {
        render_system.processEntity(entity);
    }
    render_system.render();

    /* Clutter flat tiles */
    render_system.prepare();
    for(auto entity: containingFloor->getClutterEntities()) {
        render_system.processEntity(entity);
    }
    render_system.render();
    
    /* Other big tiles */
    render_system.prepare();
    for(auto entity: containingFloor->getBiggerEntities()) {
        render_system.processEntity(entity);
    }
    render_system.render(true);

    /* Debug shapes */
    render_system.prepare();
    for(auto collisionRect: collisionsSystem.getLastCheckResults()) {
        render_system.renderTranslucentFilledBox(collisionRect, 0.0F, 1.0F, 1.0F, 0.2F);
    }
}
