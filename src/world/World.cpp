#include "World.h"
#include <iostream>
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include "LocationComponent.h"
#include <algorithm>

void Floor::addEntitysComponentsToRegisters(Entity* e) {
    CollisionComponent* cc = e->getComponent<CollisionComponent>();
    if(cc) {
        collisionComponentsRegister.push_back(cc);
    }
    MovementComponent* mc = e->getComponent<MovementComponent>();
    if(mc) {
        movementComponentsRegister.push_back(mc);
    }
    LocationComponent* lc = e->getComponent<LocationComponent>();
    if(lc) {
        locationComponentsRegister.push_back(lc);
    }
}

void Floor::addFloorEntity(Entity* e) {
    floorEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}

void Floor::addClutterEntity(Entity* e) {
    clutterEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}

void Floor::addStaticEntity(Entity* e) {
    staticEntities.push_back(e);
    biggerEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}

void Floor::addDynamicEntity(Entity* e) {
    dynamicEntities.push_back(e);
    biggerEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}


bool Floor::removeEntity(Entity* e) {
    /* Find entity in any container */
    auto floorEntityIt = std::find(floorEntities.begin(), floorEntities.end(), e);
    auto clutterEntityIt = std::find(clutterEntities.begin(), clutterEntities.end(), e);
    auto staticEntityIt = std::find(staticEntities.begin(), staticEntities.end(), e);
    auto dynamicEntityIt = std::find(dynamicEntities.begin(), dynamicEntities.end(), e);
    auto biggerEntityIt = std::find(biggerEntities.begin(), biggerEntities.end(), e);

    /* If entity not found in any container, return false */
    if(floorEntityIt == floorEntities.end() && clutterEntityIt == clutterEntities.end() 
        && staticEntityIt == staticEntities.end() && dynamicEntityIt == dynamicEntities.end()
        && biggerEntityIt == biggerEntities.end()) {
        return false;
    }

    /* Remove entity from any container */
    if(floorEntityIt != floorEntities.end()) {
        floorEntities.erase(floorEntityIt);
    }
    if(clutterEntityIt != clutterEntities.end()) {
        clutterEntities.erase(clutterEntityIt);
    }
    if(staticEntityIt != staticEntities.end()) {
        staticEntities.erase(staticEntityIt);
    }
    if(dynamicEntityIt != dynamicEntities.end()) {
        dynamicEntities.erase(dynamicEntityIt);
    }
    if(biggerEntityIt != biggerEntities.end()) {
        biggerEntities.erase(biggerEntityIt);
    }

    /* Remove entity's components from registers.
     * Find them by checking their parent entity's pointer */
    
    /* CollisionComponent */
    auto collisionCmpFindResult = std::find_if(collisionComponentsRegister.begin(), collisionComponentsRegister.end(), 
        [e](CollisionComponent* cc) {
            return cc->parent == e;
        });
    
    if(collisionCmpFindResult != collisionComponentsRegister.end()) {
        collisionComponentsRegister.erase(collisionCmpFindResult);
    }

    /* MovementComponent */
    auto movementCmpFindResult = std::find_if(movementComponentsRegister.begin(), movementComponentsRegister.end(), 
        [e](MovementComponent* mc) {
            return mc->parent == e;
        });

    if(movementCmpFindResult != movementComponentsRegister.end()) {
        movementComponentsRegister.erase(movementCmpFindResult);
    }

    /* LocationComponent */
    auto locationCmpFindResult = std::find_if(locationComponentsRegister.begin(), locationComponentsRegister.end(), 
        [e](LocationComponent* lc) {
            return lc->parent == e;
        });

    if(locationCmpFindResult != locationComponentsRegister.end()) {
        locationComponentsRegister.erase(locationCmpFindResult);
    }

    return true;
}



/* World */

int World::getFloorsCount() const {
    return static_cast<int>(floors.size());
}

void World::appendFloor(int elevation) {
    floors.push_back(Floor(elevation, this));
}

std::optional<Floor*> World::getFloor(int elevation) {
    if(elevation < 0 || elevation >= static_cast<int>(floors.size())) {
        return std::nullopt;
    }
    return &floors[elevation];
}

std::optional<Floor*> World::getTopFloor() {
    if(floors.empty()) {
        return std::nullopt;
    }

    return &floors.back();
}

bool World::moveEntityToFloor(Entity* e, int newElevation) {
    return false; //todo
}