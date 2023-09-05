#include "World.h"
#include <iostream>
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <algorithm>


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


std::optional<Elevation*> World::getLowerElevation(Elevation* elevation) {
    auto elevationIndex = elevation->getIndex();
    if(elevationIndex == 0) {
        return std::nullopt;
    }
    return getElevationOption(elevationIndex - 1);
}

std::optional<Elevation*> World::getHigherElevation(Elevation* elevation) {
    auto elevationIndex = elevation->getIndex();
    if(elevationIndex == getElevationsCount() - 1) {
        return std::nullopt;
    }
    return getElevationOption(elevationIndex + 1);
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