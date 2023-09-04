#include "World.h"
#include <iostream>
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <algorithm>

/* Tiles Pair/Quad */

// TilesPair TilesQuad::getFrontTilesRelativeToDirection(Vect2F direction) const {
//     if(direction.x > 0) {
//         return TilesPair(topRightGlobal, bottomRightGlobal);
//     } 
//     else if(direction.x < 0) {
//         return TilesPair(bottomLeftGlobal, topLeftGlobal);
//     } 
//     else if(direction.y < 0) {
//         return TilesPair(topLeftGlobal, topRightGlobal);
//     } 
//     else if(direction.y > 0) {
//         return TilesPair(bottomRightGlobal, bottomLeftGlobal);
//     }
//     /* No direction same as direction (0, 1)*/
//     return TilesPair(bottomRightGlobal, bottomLeftGlobal);
// }

// Rect4F TilesPair::getBoundingRect() const {
//     if(!hasAny()) {
//         return Rect4F{0,0,0,0};
//     }

//     Entity* first = left != nullptr ? left : right;
//     /* First is for sure not null */
    
//     Entity* second = left != nullptr ? right : left;
//     if(!second) {
//         second = first;
//     }
//     /* If second was null, now they are the same for easier calc */

//     float leftValue = std::min(first->getCuboid().value().left(), second->getCuboid().value().left());
//     float rightValue = std::max(first->getCuboid().value().right(), second->getCuboid().value().right());
//     float topValue = std::min(first->getCuboid().value().top(), second->getCuboid().value().top());
//     float bottomValue = std::max(first->getCuboid().value().bottom(), second->getCuboid().value().bottom());

//     return Rect4F{leftValue, topValue, rightValue - leftValue, bottomValue - topValue};
// }


/* Elevation class */

Cuboid6F Elevation::elevationToWorldSpace(const Cuboid6F& elevationSpaceCuboid) const {
    return elevationSpaceCuboid.getTranslated(Vect3F{0, 0, this->z()});
}

void Elevation::addEntitisComponentsToRegisters(Entity* entity) {
    CollisionComponent* cc = entity->getComponent<CollisionComponent>();
    if(cc) {
        collisionComponentsRegister.push_back(cc);
    }
    MovementComponent* mc = entity->getComponent<MovementComponent>();
    if(mc) {
        movementComponentsRegister.push_back(mc);
    }
}

void Elevation::moveEntityToElevationOrThrow(Entity* entity, Elevation* nextElevation) {
    /* Check if entity is in this elevation */
    auto entityIt = std::find(allEntitiesRegister.begin(), allEntitiesRegister.end(), entity);
    if(entityIt == allEntitiesRegister.end()) {
        std::cerr << "Elevation::moveEntityToElevationOrThrow: Entity not found in this elevation" << std::endl;
        throw std::invalid_argument("Elevation::moveEntityToElevationOrThrow: Entity not found in this elevation");
    }

    /* Deregister from current elevation */
    deregisterEntityOrThrow(entity);

    /* Register in next elevation */
    nextElevation->registetedEntityOrThrow(entity);
    std::cout << "Entity moved to elevation " << nextElevation->getIndex() << std::endl;
}

Entity* Elevation::createEntityOrThrow(EntityType type) {
    /* Add entity to proper container */
    Entity* newEntity = nullptr;
    try {
        newEntity = new Entity(this, type);
    } catch(std::bad_alloc& e) {
        std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
        throw e;
    }

    try {
        registetedEntityOrThrow(newEntity);
    } catch(std::invalid_argument& e) {
        std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
        throw std::bad_alloc();
    }

    return newEntity;
}

void Elevation::registetedEntityOrThrow(Entity* newEntity) {
    /* Check if entity already registered in world */
    auto entityIt = std::find(containingWorld.allEntitiesRegisterBegin(), containingWorld.allEntitiesRegisterEnd(), newEntity);
    if(entityIt != containingWorld.allEntitiesRegisterEnd()) {
        std::cerr << "Elevation::addEntity: Entity already registered in world" << std::endl;
        throw std::invalid_argument("Elevation::addEntity: Entity already registered in world");
    }

    /* Assign to specific group */
    switch(newEntity->getType()) {
        case EntityType::FLOOR:
            floorEntities.push_back(newEntity);
            break;
        case EntityType::CLUTTER:
            clutterEntities.push_back(newEntity);
            break;
        case EntityType::STATIC:
            staticEntities.push_back(newEntity);
            biggerEntitiesRegister.push_back(newEntity);
            break;
        case EntityType::DYNAMIC:
            dynamicEntities.push_back(newEntity);
            biggerEntitiesRegister.push_back(newEntity);
            break;
        default:
            std::cout << "Elevation::addEntity: Unknown entity type" << std::endl;
            throw std::invalid_argument("Elevation::addEntity: Unknown entity type");
            break;
    }

    addEntitisComponentsToRegisters(newEntity);
    
    /* Finally add to all elevations entities register */
    allEntitiesRegister.push_back(newEntity);

    newEntity->setElevation(this);
}

void Elevation::deregisterEntityOrThrow(Entity* entity) {
    /* Find entity in any container */
    auto floorEntityIt = std::find(floorEntities.begin(), floorEntities.end(), entity);
    auto clutterEntityIt = std::find(clutterEntities.begin(), clutterEntities.end(), entity);
    auto staticEntityIt = std::find(staticEntities.begin(), staticEntities.end(), entity);
    auto dynamicEntityIt = std::find(dynamicEntities.begin(), dynamicEntities.end(), entity);

    /* If entity not found in any container, return false */
    if(floorEntityIt == floorEntities.end() 
        && clutterEntityIt == clutterEntities.end() 
        && staticEntityIt == staticEntities.end() 
        && dynamicEntityIt == dynamicEntities.end()) {
        std::cerr << "Elevation::deregisterEntityOrThrow: Entity not found in any container" << std::endl;
        throw std::invalid_argument("Elevation::deregisterEntityOrThrow: Entity not found in any container");
    }

    /* Remove entity from registers */
    auto biggerEntityIt = std::find(biggerEntitiesRegister.begin(), biggerEntitiesRegister.end(), entity);
    if(biggerEntityIt != biggerEntitiesRegister.end()) {
        biggerEntitiesRegister.erase(biggerEntityIt);
    }

    auto allEntityIt = std::find(allEntitiesRegister.begin(), allEntitiesRegister.end(), entity);
    if(allEntityIt != allEntitiesRegister.end()) {
        allEntitiesRegister.erase(allEntityIt);
    }

    /* Remove entity's components from registers.
     * Find them by checking their parent entity's pointer 
    */
    
    /* CollisionComponent */
    auto collisionCmpFindResult = std::find_if(collisionComponentsRegister.begin(), collisionComponentsRegister.end(), 
        [entity](CollisionComponent* cc) {
            return cc->getParentEntity() == entity;
        });
    
    if(collisionCmpFindResult != collisionComponentsRegister.end()) {
        collisionComponentsRegister.erase(collisionCmpFindResult);
    }

    /* MovementComponent */
    auto movementCmpFindResult = std::find_if(movementComponentsRegister.begin(), movementComponentsRegister.end(), 
        [entity](MovementComponent* mc) {
            return mc->getParentEntity() == entity;
        });

    if(movementCmpFindResult != movementComponentsRegister.end()) {
        movementComponentsRegister.erase(movementCmpFindResult);
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

    entity->setElevation(nullptr);
}

bool Elevation::deleteEntityIfExists(Entity* entity) {
    try {
        deregisterEntityOrThrow(entity);
    } catch(std::invalid_argument& e) {
        std::cerr << "Elevation::deleteEntityIfExists: " << e.what() << std::endl;
        return false;
    }
    /*
     * Seems entity registers and all its component are removed.
        * Now delete entity.
    */
    delete entity;

    return true;
}


/* Other */

Entity* Elevation::getFloorEntityByXY(const Vect2F& entityPosition) {
    for(auto entity : floorEntities) {
        if(entity->getCuboid().value().getFlatten().hasPointInside(entityPosition)) {
            return entity;
        }
    }
    return nullptr;
}

// TilesQuad Elevation::getFloorNearbyTilesQuad(const Rect4F& entityRect) {
//     TilesQuad tilesQuad{
//         getFloorEntityByXY(entityRect.topLeft()),
//         getFloorEntityByXY(entityRect.topRight()),
//         getFloorEntityByXY(entityRect.bottomLeft()),
//         getFloorEntityByXY(entityRect.bottomRight())
//     };

//     return tilesQuad;
// }

std::vector<Entity*> Elevation::getAnyIntersectingEntities(Vect2F pointElevationSpace) {
    std::vector<Entity*> result;

    std::copy_if(allEntitiesRegister.begin(), allEntitiesRegister.end(), std::back_inserter(result), 
        [pointElevationSpace](Entity* e) {
            return e->getCuboid().value().getFlatten().hasPointInside(pointElevationSpace);
        });

    return result;
}

std::vector<Entity*> Elevation::getAnyIntersectingEntities(const Rect4F& rectElevationSpace, Vect2F direction) {
    /* Find 2 vertices pointer by direction */
    Vect2F firstVertex;
    Vect2F secondVertex;
    if(direction.x > 0) {
        firstVertex = rectElevationSpace.topRight();
        secondVertex = rectElevationSpace.bottomRight();
    } 
    else if(direction.x < 0) {
        firstVertex = rectElevationSpace.bottomLeft();
        secondVertex = rectElevationSpace.topLeft();
    } 
    else if(direction.y < 0) {
        firstVertex = rectElevationSpace.topLeft();
        secondVertex = rectElevationSpace.topRight();
    } 
    else if(direction.y > 0) {
        firstVertex = rectElevationSpace.bottomRight();
        secondVertex = rectElevationSpace.bottomLeft();
    }

    //todo refactor

    /* Find entities colliding with first vertex */
    auto firstVertexCollidingEntities = getAnyIntersectingEntities(firstVertex);

    /* Find entities colliding with second vertex */
    auto secondVertexCollidingEntities = getAnyIntersectingEntities(secondVertex);

    /* Combine results */
    std::vector<Entity*> result;
    // std::copy(firstVertexCollidingEntities.begin(), firstVertexCollidingEntities.end(), std::back_inserter(result));
    // std::copy(secondVertexCollidingEntities.begin(), secondVertexCollidingEntities.end(), std::back_inserter(result));

    /* Pass only unique */
    for(Entity* e : firstVertexCollidingEntities) {
        if(std::find(result.begin(), result.end(), e) == result.end()) {
            result.push_back(e);
        }
    }

    for(Entity* e : secondVertexCollidingEntities) {
        if(std::find(result.begin(), result.end(), e) == result.end()) {
            result.push_back(e);
        }
    }

    return result;
}



/* World class */

int World::getElevationsCount() const {
    return static_cast<int>(elevations.size());
}

Elevation* World::createElevationOrThrow() {
    int nextElevationIndex = getElevationsCount();
    Elevation* nextElevation = nullptr;
    try {
        nextElevation = new Elevation(nextElevationIndex, *this);
    } catch (std::bad_alloc& e) {
        std::cerr << "World failed to allocate Elevation: " << e.what() << std::endl;
        throw e;
    }
    elevations.push_back(nextElevation);
    return nextElevation;
}

std::optional<Elevation*> World::getElevationOption(int elevation) {
    if(elevation < 0 || elevation >= static_cast<int>(elevations.size())) {
        return std::nullopt;
    }
    return elevations[elevation];
}

std::optional<Elevation*> World::getTopElevationOption() {
    if(elevations.empty()) {
        return std::nullopt;
    }
    return elevations.back();
}

Elevation& World::operator[](int elevation) {
    auto elevationsCount = getElevationsCount();
    if(elevation < 0 || elevation >= elevationsCount) {
        throw std::out_of_range("World::operator[]: Elevation index out of range");
    }
    return *elevations[elevation];
}

const Elevation& World::operator[](int elevation) const {
        auto elevationsCount = getElevationsCount();
    if(elevation < 0 || elevation >= elevationsCount) {
        throw std::out_of_range("World::operator[]: Elevation index out of range");
    }
    return *elevations[elevation];
}


Elevation& World::getTopElevationOrThrow() {
    if(elevations.empty()) {
        throw std::out_of_range("World::getTopElevationOrThrow: No elevations");
    }

    return *elevations.back();
}

const Elevation& World::getTopElevationConstOrThrow() {
    if(elevations.empty()) {
        throw std::out_of_range("World::getTopElevationOrThrow: No elevations");
    }

    return *elevations.back();
}

/* Iterators */


std::vector<Elevation*>::iterator World::begin() {
    return elevations.begin();
}

std::vector<Elevation*>::iterator World::end() {
    return elevations.end();
}

std::vector<Elevation*>::const_iterator World::begin() const {
    return elevations.begin();
}

std::vector<Elevation*>::const_iterator World::end() const {
    return elevations.end();
}

/* Other */

void World::moveDynamicEntityToElevationOrThrow(Entity* e, int destinationElevationIndex) {
    /* Entity should have some layer - it seems error-free */
    auto oldElevation = e->getContainingElevationOrThrow();
    auto nextElevation = &(*this)[destinationElevationIndex];

    /* Delegate this task to Elevations */
    oldElevation->moveEntityToElevationOrThrow(e, nextElevation);
}

/* 
 * Creation
*/

Entity* World::createEntityOnElevationOrThrow(int elevationIndex, EntityType type) {
    try {
        auto& containingElevation = (*this)[elevationIndex];
        return createEntityOnElevationOrThrow(&containingElevation, type);
    } catch (std::out_of_range& e) {
        std::cerr << "World::createEntityOnElevationOrThrow: " << e.what() << std::endl;
        throw e;
    }
}

Entity* World::createEntityOnElevationOrThrow(Elevation* elevation, EntityType type) {
    Entity* newEntity = nullptr;
    try {
        newEntity = elevation->createEntityOrThrow(type);
    } catch(std::bad_alloc& e) {
        std::cerr << "World::createEntityOnElevationOrThrow: " << e.what() << std::endl;
        throw e;
    }
    allEntitiesRegister.push_back(newEntity);
    return newEntity;
}

/* 
 * Destruction
*/

void World::deleteEntityOrThrow(Entity* e) {
    /* Find entity in any container */
    auto entityIt = std::find(allEntitiesRegister.begin(), allEntitiesRegister.end(), e);

    /* If entity not found in any container, return false */
    if(entityIt == allEntitiesRegister.end()) {
        throw std::invalid_argument("World::deleteEntityOrThrow: Entity not found in World");
    }

    /* Remove entity from container */
    allEntitiesRegister.erase(entityIt);

    /* Remove entity from its elevation.
    Elevation takes care of deletion. */
    auto deletionResult = e->getContainingElevationOrThrow()->deleteEntityIfExists(e);

    if(!deletionResult) {
        throw std::invalid_argument("World::deleteEntityOrThrow: Entity not found in its elevation");
    }
}


std::vector<Entity*> World::getAnyIntersectingEntities(Vect2F pointElevationSpace, int elevationIndex) {
    try {
        return (*this)[elevationIndex].getAnyIntersectingEntities(pointElevationSpace);
    } catch(std::out_of_range&) {
        return std::vector<Entity*>();
    }
}