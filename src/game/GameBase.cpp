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
        auto playersElevation = player->getContainingElevationOrThrow();

        std::cout << " _______________________________________________________________" << std::endl;
        std::cout << "| Player location world space: " << player->getCuboidWorldSpace().topLeft << ", worldSpaceZ: " << playersElevation->getWorldSpaceZ() << std::endl;
        std::cout << "| Player CuboidWorldSpace: " << player->getCuboidWorldSpace() << " flatten to: " << player->getCuboidWorldSpace().getFlatten() << std::endl;
        std::cout << "| Player CuboidElevationSpace: " << player->getCuboidElevationSpace() << " flatten to: " << player->getCuboidElevationSpace().getFlatten() << std::endl;
        std::cout << "| Last loop rendered entities: " << render_system.getLastLoopEntitesCount() << std::endl;
        std::cout << "| Elevation index:" << playersElevation->getIndex() << ", elevations count: " << world.getElevationsCount() << ", this info:" << std::endl;
        std::cout << "|  - entities floor:   " << playersElevation->getFloorEntitiesCount() << std::endl;
        std::cout << "|  - entities clutter: " << playersElevation->getClutterEntitiesCount() << std::endl;
        std::cout << "|  - entities static:  " << playersElevation->getStaticEntitiesCount() << std::endl;
        std::cout << "|  - entities dynamic: " << playersElevation->getDynamicEntitiesCount() << std::endl;
        std::cout << "|  - entities bigger:  " << playersElevation->getBiggerEntitiesCount() << std::endl;
        std::cout << "|  - components movement: " << playersElevation->getMovementComponentsCount() << std::endl;
        std::cout << "|  - components collision: " << playersElevation->getCollisionComponentsCount() << std::endl;
        std::cout << "| WorldClipRect: " << render_system.getRenderBoxWorldSpace() << std::endl;
        
        std::cout << "| Elevations data: " << std::endl;
        for(auto elevation : world) {
            std::cout << "|  - elevation index: " << elevation->getIndex() << ", worldSpaceZ: " << elevation->getWorldSpaceZ() << std::endl;
        }
        
        std::cout << std::endl;
        return true;
    }
    
    if(key == GLFW_KEY_O) {
        /* Toggle debug view */
        debugView = !debugView;
        render_system.debugView = debugView;
        return true;
    }
    
    if(key == GLFW_KEY_UP) {
        render_system.setClipElevationIndex(render_system.getClipElevationIndex() + 1);
        
        std::cout << "Clip elevation index: " << render_system.getClipElevationIndex() << " out of: ";
        for(auto elevation : world) {
            std::cout << elevation->getIndex() << ", ";
        }
        std::cout << std::endl;
        
        return true;
    }
    
    if(key == GLFW_KEY_DOWN) {
        render_system.setClipElevationIndex(render_system.getClipElevationIndex() - 1);
        
        std::cout << "Clip elevation index: " << render_system.getClipElevationIndex() << " out of: ";
        for(auto elevation : world) {
            std::cout << elevation->getIndex() << ", ";
        }
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

    auto containingFloor = world.player.getContainingElevationOrThrow();

    /**
     * Update movement is for dynamicEntities only.
     * Player is also dynamicEntity.
     */
    for(auto dynamicEntityIt = containingFloor->dynamicEntitiesBegin(); dynamicEntityIt != containingFloor->dynamicEntitiesEnd(); ++dynamicEntityIt) {
        auto dynamicEntity = *dynamicEntityIt;
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
        
    collisionsSystem.update(containingFloor->collisionComponentsRegisterBegin(), containingFloor->collisionComponentsRegisterEnd(), &world.player, dt);
    
    camera.update(dt);
}

void GameBase::render() {
    render_system.loopStart();
    auto containingFloor = world.player.getContainingElevationOrThrow();

    for(auto elevation : world) {
    render_system.batchStart();
        //todo probably can be const
        if(elevation->getIndex() == world.player.getContainingElevationOrThrow()->getIndex()) {
            const float minOpacity = 0.2F;
            const float opacity = minOpacity +  (1.0F - minOpacity) * elevation->getWorldSpaceZ() / static_cast<float>(world.getElevationsCount());
            render_system.fillScreen(0.0F, 0.0F, 0.0F, opacity);
        }
        render_system.batchAppendElevation(elevation);
        render_system.batchEnd();
        render_system.renderBatch();
    }

    /* Debug shapes */
    if(debugView) {
        render_system.renderCollisionBoxes(containingFloor->collisionComponentsRegisterBegin(), containingFloor->collisionComponentsRegisterEnd());

        render_system.renderTranslucentFilledBox(world.player.collisionDetectorComponent->getElevationSpaceBoundingRect(), 0.1F, 0.2F, 1.0F, 0.5F);

        for(auto collisionRectIt = collisionsSystem.getCollidingRectsBegin(), end = collisionsSystem.getCollidingRectsEnd(); collisionRectIt != end; ++collisionRectIt) {
            const auto& collisionRect = *collisionRectIt;
            render_system.renderTranslucentFilledBox(collisionRect, 0.0F, 1.0F, 1.0F, 0.2F);
        }
    }

    render_system.loopEnd();
}
