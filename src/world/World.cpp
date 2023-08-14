#include "World.h"
#include <iostream>
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <algorithm>

void Elevation::addEntitisComponentsToRegisters(Entity* e) {
    CollisionComponent* cc = e->getComponent<CollisionComponent>();
    if(cc) {
        collisionComponentsRegister.push_back(cc);
    }
    MovementComponent* mc = e->getComponent<MovementComponent>();
    if(mc) {
        movementComponentsRegister.push_back(mc);
    }
}

void Elevation::addFloorEntity(Entity* e) {
    floorEntities.push_back(e);
    addEntitisComponentsToRegisters(e); //todo remove - on start nothing inside
}

void Elevation::addClutterEntity(Entity* e) {
    clutterEntities.push_back(e);
    addEntitisComponentsToRegisters(e);
}

void Elevation::addStaticEntity(Entity* e) {
    staticEntities.push_back(e);
    biggerEntitiesRegister.push_back(e);
    addEntitisComponentsToRegisters(e);
}

void Elevation::addDynamicEntity(Entity* e) {
    dynamicEntities.push_back(e);
    biggerEntitiesRegister.push_back(e);
    addEntitisComponentsToRegisters(e);
}


bool Elevation::deleteEntity(Entity* e) {
    /* Find entity in any container */
    auto floorEntityIt = std::find(floorEntities.begin(), floorEntities.end(), e);
    auto clutterEntityIt = std::find(clutterEntities.begin(), clutterEntities.end(), e);
    auto staticEntityIt = std::find(staticEntities.begin(), staticEntities.end(), e);
    auto dynamicEntityIt = std::find(dynamicEntities.begin(), dynamicEntities.end(), e);
    auto biggerEntityIt = std::find(biggerEntitiesRegister.begin(), biggerEntitiesRegister.end(), e);

    /* If entity not found in any container, return false */
    if(floorEntityIt == floorEntities.end() 
        && clutterEntityIt == clutterEntities.end() 
        && staticEntityIt == staticEntities.end() 
        && dynamicEntityIt == dynamicEntities.end()
        && biggerEntityIt == biggerEntitiesRegister.end()) {
        return false;
    }

    /* Remove entity from container */
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
    if(biggerEntityIt != biggerEntitiesRegister.end()) {
        biggerEntitiesRegister.erase(biggerEntityIt);
    }

    /* Remove entity's components from registers.
     * Find them by checking their parent entity's pointer 
    */
    
    /* CollisionComponent */
    auto collisionCmpFindResult = std::find_if(collisionComponentsRegister.begin(), collisionComponentsRegister.end(), 
        [e](CollisionComponent* cc) {
            return cc->getParentEntity() == e;
        });
    
    if(collisionCmpFindResult != collisionComponentsRegister.end()) {
        collisionComponentsRegister.erase(collisionCmpFindResult);
    }

    /* MovementComponent */
    auto movementCmpFindResult = std::find_if(movementComponentsRegister.begin(), movementComponentsRegister.end(), 
        [e](MovementComponent* mc) {
            return mc->getParentEntity() == e;
        });

    if(movementCmpFindResult != movementComponentsRegister.end()) {
        movementComponentsRegister.erase(movementCmpFindResult);
    }

    /*
     *Seems entity and all its component are removed
    */
    return true;
}



/* World */

int World::getElevationsCount() const {
    return static_cast<int>(elevations.size());
}

Elevation* World::appendElevation() {
    int nextElevation = getElevationsCount();
    elevations.push_back(Elevation(nextElevation, this));
    return &elevations.back();
}

std::optional<Elevation*> World::getElevation(int elevation) {
    if(elevation < 0 || elevation >= static_cast<int>(elevations.size())) {
        return std::nullopt;
    }

    return &elevations[elevation];
}

std::optional<Elevation*> World::getTopElevation() {
    if(elevations.empty()) {
        return std::nullopt;
    }

    return &elevations.back();
}

bool World::moveDynamicEntityToElevation(Entity* e, int destinationElevationIndex) {
    /* Entity should have some layer - it seems error-free */
    auto oldElevation = e->getContainingElevation();
    if(!oldElevation) {
        return false;
    }

    /* Entity has to be in dynamic container */
    auto dynamicEntitiesVector = oldElevation->getDynamicEntities();
    auto entityIt = std::find(dynamicEntitiesVector.begin(), dynamicEntitiesVector.end(), e);
    if(entityIt == dynamicEntitiesVector.end()) {
        return false;
    }

    /* Find destination elevation */
    auto destinationElevationOption = getElevation(destinationElevationIndex);
    if(!destinationElevationOption || (destinationElevationOption == oldElevation)) {
        return false;
    }
    auto destinationElevation = destinationElevationOption.value();

    /* Remove entity from old elevation */
    oldElevation->deleteEntity(e);

    /* Add dynamic entity to new elevation */
    destinationElevation->addDynamicEntity(e);

    /* Only pointer is passed between elevations. Inside World structures it stays the same. */
    return true;
}

/* 
 * Creation
*/

std::optional<Entity *> World::createFloorEntity(int elevation) {
    auto containingElevationOption = getElevation(elevation);
    if(!containingElevationOption) {
        return std::nullopt;
    }

    return createFloorEntity(*containingElevationOption);
}

std::optional<Entity *> World::createFloorEntity(Elevation* elevation) {
    Entity* e = new Entity(elevation, EntityType::FLOOR);
    elevation->addFloorEntity(e);
    allEntities.push_back(e);
    return e;
}

std::optional<Entity *> World::createClutterEntity(int elevation) {
    auto containingElevationOption = getElevation(elevation);
    if(!containingElevationOption) {
        return std::nullopt;
    }

    return createClutterEntity(*containingElevationOption);
}

std::optional<Entity *> World::createClutterEntity(Elevation* elevation) {
    Entity* e = new Entity(elevation, EntityType::CLUTTER);
    elevation->addClutterEntity(e);
    allEntities.push_back(e);
    return e;
}

std::optional<Entity *> World::createStaticEntity(int elevation) {
    auto containingElevationOption = getElevation(elevation);
    if(!containingElevationOption) {
        return std::nullopt;
    }

    return createStaticEntity(*containingElevationOption);
}

std::optional<Entity *> World::createStaticEntity(Elevation* elevation) {
    Entity* e = new Entity(elevation, EntityType::STATIC);
    elevation->addStaticEntity(e);
    allEntities.push_back(e);
    return e;
}

std::optional<Entity *> World::createDynamicEntity(int elevation) {
    auto containingElevationOption = getElevation(elevation);
    if(!containingElevationOption) {
        return std::nullopt;
    }

    return createDynamicEntity(*containingElevationOption);
}

std::optional<Entity *> World::createDynamicEntity(Elevation* elevation) {
    Entity* e = new Entity(elevation, EntityType::DYNAMIC);
    elevation->addDynamicEntity(e);
    allEntities.push_back(e);
    return e;
}

/* 
 * Destruction
*/

bool World::deleteEntity(Entity* e) {
    /* Find entity in any container */
    auto entityIt = std::find(allEntities.begin(), allEntities.end(), e);

    /* If entity not found in any container, return false */
    if(entityIt == allEntities.end()) {
        return false;
    }

    /* Remove entity from container */
    allEntities.erase(entityIt);

    /* Remove entity from its elevation */
    e->getContainingElevation()->deleteEntity(e);

    /* Delete entity */
    delete e;

    return true;
}