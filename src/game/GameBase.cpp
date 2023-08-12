#include "GameBase.h"
#include <iostream>
#include "Window.h"
#include "Callbacks.h"
#include "Loader.h"
#include "IO.h"
#include "World.h"
#include "Component.h"
#include "ControllableComponent.h"
#include "CollisionComponents.h"

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
    camera.focusOn(&world.player);

    /* Attach controller to just loaded player */
    userInputSystem.attachControllabe(&world.player);
    
    return result;
}

bool GameBase::onKeyReleased(int key) {
    if(key == GLFW_KEY_I) {
        /* Print some informations */
        auto player = &world.player;
        auto playersElevation = player->getContainingElevation();

        std::cout << " __________________________________________________________" << std::endl;
        std::cout << "| Player location: " << player->getPositionWorldSpace() << "/" << playersElevation->getWorldSpaceZ() << std::endl;
        std::cout << "| Render batch size: " << render_system.getLastEntitesCount() << std::endl;
        std::cout << "| Floor [" << playersElevation->getIndex() << "/" << world.getElevationsCount() << "] info:" << std::endl;
        std::cout << "|  - entities floor:   " << playersElevation->getFloorEntities().size() << std::endl;
        std::cout << "|  - entities clutter: " << playersElevation->getClutterEntities().size() << std::endl;
        std::cout << "|  - entities static:  " << playersElevation->getStaticEntities().size() << std::endl;
        std::cout << "|  - entities dynamic: " << playersElevation->getDynamicEntities().size() << std::endl;
        std::cout << "|  - entities bigger:  " << playersElevation->getBiggerEntities().size() << std::endl;
        std::cout << "|  - components movement: " << playersElevation->getMovementComponents().size() << std::endl;
        std::cout << "|  - components collision: " << playersElevation->getCollisionComponents().size() << std::endl;
        std::cout << "| WorldClipRect: " << render_system.renderBoxWorldSpace << std::endl;

        
        std::cout << std::endl;
        return true;
    }
    
    if(key == GLFW_KEY_O) {
        /* Toggle debug view */
        debugView = !debugView;
        render_system.rendeDebugView = debugView;
        return true;
    }

    return false;
}


bool GameBase::input() {
    // user_input_system.process_keyboard();
    return false;
}

void GameBase::update(float dt) {
    // auto containingFloorOption = world.player.getFloor();
    // if(!containingFloorOption.has_value()) {
    //     std::cerr << "Player is not on floor!" << std::endl;
    //     return;
    // }
    //todo apply to all floor with common function
    auto containingFloor = world.player.getContainingElevation();

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
    // auto containingFloorOption = world.player.getFloor();
    // if(!containingFloorOption.has_value()) {
    //     std::cerr << "Player is not on floor!" << std::endl;
    //     return;
    // }
    auto containingFloor = world.player.getContainingElevation();
    

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
    if(debugView) {
        render_system.prepare();
        render_system.renderCollisionBoxes(containingFloor->getCollisionComponents());
        //->getWorldSpaceBoundingRect()
        render_system.renderTranslucentFilledBox(world.player.collisionDetectorComponent->getElevationSpaceBoundingRect(), 0.1F, 0.2F, 1.0F, 0.5F);

        for(auto collisionRect: collisionsSystem.getLastCheckResults()) {
            render_system.renderTranslucentFilledBox(collisionRect, 0.0F, 1.0F, 1.0F, 0.2F);
        }
    }
}
