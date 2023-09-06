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
    camera.focusOn(&world.getPlayer());

    /* Attach controller to just loaded player */
    userInputSystem.attachControllabe(&world.getPlayer());
    
    return result;
}

bool GameBase::onKeyReleased(int key) {
    if(key == GLFW_KEY_I) {
        /* Print some informations */
        auto player = &world.getPlayer();
        auto playersElevation = player->getContainingElevationOrThrow();

        std::cout << " _______________________________________________________________" << std::endl;
        std::cout << "| Player location world space: " << player->getCuboid().toWorldSpace().value().topLeftBottom() << ", worldSpaceZ: " << playersElevation->z() << std::endl;
        std::cout << "| Player CuboidWorldSpace: " << player->getCuboid().toWorldSpace().value() << " flatten to: " << player->getCuboid().toWorldSpace().value().getFlatten() << std::endl;
        std::cout << "| Player CuboidElevationSpace: " << player->getCuboid().value() << " flatten to: " << player->getCuboid().value().getFlatten() << std::endl;
        std::cout << "| Last loop rendered entities: " << render_system.getLastLoopEntitesCount() << std::endl;
        std::cout << "| Elevation index:" << playersElevation->getIndex() << ", elevations count: " << world.getElevationsCount() << ", this info:" << std::endl;
        for(const auto entityType: Entity::AllTypes) {
            std::cout << "|  - entities " << Entity::typeToString(entityType) << ":   " << playersElevation->getEntitiesCount(entityType) << std::endl;
        }
        std::cout << "|  - entities BIGGER:  " << playersElevation->getBiggerEntitiesCount() << std::endl;
        std::cout << "|  - components movement: " << playersElevation->getCollisionComponentsCount() << std::endl;
        std::cout << "|  - components collision: " << playersElevation->getMovementComponentsCount() << std::endl;
        std::cout << "| WorldClipRect: " << render_system.getRenderBoxWorldSpace() << std::endl;
        std::cout << "| Loaded tilesets count: " << Loader::getLoader().getLoadedTilesetsCount() << std::endl;
        std::cout << "| Loaded tile datas count: " << Loader::getLoader().getLoadedTilesDataCount() << std::endl;
        
        std::cout << "| Elevations data: " << std::endl;
        for(auto elevation : world) {
            std::cout << "|  - elevation index: " << elevation->getIndex() << ", worldSpaceZ: " << elevation->z() << std::endl;
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

    auto containingFloor = world.getPlayer().getContainingElevationOrThrow();

    /**
     * Update movement is for dynamicEntities only.
     * Player is also dynamicEntity.
     */
    for(auto dynamicEntityIt = containingFloor->entitiesBegin(Entity::Type::DYNAMIC); dynamicEntityIt != containingFloor->entitiesEnd(Entity::Type::DYNAMIC); ++dynamicEntityIt) {
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
        
    collisionsSystem.startSession();
    collisionsSystem.processDetector(&world.getPlayer(), containingFloor->collisionComponentsBegin(), containingFloor->collisionComponentsEnd());

    camera.update(dt);
}

void GameBase::render() {
    render_system.loopStart();
    auto containingFloor = world.getPlayer().getContainingElevationOrThrow();

    for(auto elevation : world) {
    render_system.batchStart();
        //todo probably can be const
        if(elevation->getIndex() == world.getPlayer().getContainingElevationOrThrow()->getIndex()) {
            const float minOpacity = 0.2F;
            const float opacity = minOpacity +  (1.0F - minOpacity) * elevation->z() / static_cast<float>(world.getElevationsCount());
            render_system.fillScreen(0.0F, 0.0F, 0.0F, opacity);
        }
        render_system.batchAppendElevation(elevation);
        render_system.batchEnd();
        render_system.renderBatch();
    }

    /* Debug shapes */
    if(debugView) {
        /* Blue - all */
        render_system.renderCollisionBoxes(containingFloor->getIndex(), containingFloor->collisionComponentsBegin(), containingFloor->collisionComponentsEnd());
        
        {
            /* Red - not walkable */
            const auto debugCuboidsWorldSpace = collisionsSystem.getNotWalkableObstaclesCuboids();
            for(const auto& debugCuboidWorldSpace :  debugCuboidsWorldSpace) {
                render_system.renderTranslucentFilledCuboid6F(debugCuboidWorldSpace.value(), 1.0F, 0.0F, 0.0F, 0.5F);
            }
        }
        
        {
            /* Green - walkable */
            const auto debugCuboidsWorldSpace = collisionsSystem.getWalkableObstaclesCuboids();
            for(const auto& debugCuboidWorldSpace :  debugCuboidsWorldSpace) {
                render_system.renderTranslucentFilledCuboid6F(debugCuboidWorldSpace.value(), 0.0F, 1.0F, 0.0F, 0.5F);
            }
        }

        {
            /* Black - entities debug */
            const auto debugCuboidsWorldSpace = collisionsSystem.getDebugResults();
            for(const auto& debugCuboidWorldSpace :  debugCuboidsWorldSpace) {
                render_system.renderTranslucentFilledCuboid6F(debugCuboidWorldSpace.value(), 0.0F, 0.0F, 0.0F, 0.5F);
            }
        }

        /* White - detector */
        const auto playerBoundingWorldSpace = world.getPlayer().getCollisionDetectorComponent()->getElevationBoundingCuboid().toWorldSpace();
        render_system.renderTranslucentFilledCuboid6F(playerBoundingWorldSpace.value(), 1.0F, 1.0F, 1.0F, 0.3F);
    }

    render_system.loopEnd();
}
