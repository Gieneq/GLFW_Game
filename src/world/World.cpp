#include "World.h"
#include <iostream>
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <algorithm>


/* Counts */

int World::getElevationsCount() const {
    return static_cast<int>(elevations.size());
}

/* Access Elevation */

std::optional<Elevation*> World::getElevationOption(int elevation) {
    if(elevation < 0 || elevation >= static_cast<int>(elevations.size())) {
        return std::nullopt;
    }
    return elevations[elevation];
}

std::optional<const Elevation*> World::getElevationOption(int elevation) const {
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

std::optional<const Elevation*> World::getLowerElevation(Elevation* elevation) const {
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

std::optional<const Elevation*> World::getHigherElevation(Elevation* elevation) const {
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

std::optional<const Elevation*> World::getTopElevationOption() const {
    if(elevations.empty()) {
        return std::nullopt;
    }
    return elevations.back();
}

Elevation* World::operator[](int elevation) {
    auto elevationsCount = getElevationsCount();
    if(elevation < 0 || elevation >= elevationsCount) {
        throw std::out_of_range("World::operator[]: Elevation index out of range");
    }
    return elevations[elevation];
}

const Elevation* World::operator[](int elevation) const {
        auto elevationsCount = getElevationsCount();
    if(elevation < 0 || elevation >= elevationsCount) {
        throw std::out_of_range("World::operator[]: Elevation index out of range");
    }
    return elevations[elevation];
}


/* Other */

void World::moveEntityToElevationOrThrow(Entity* entity, int destinationElevationIndex) {
    /* Entity should have some layer - it seems error-free */
    auto oldElevation = entity->getContainingElevationOrThrow();
    auto nextElevation = (*this)[destinationElevationIndex];

    /* Check if is already on destination elevation */
    if(oldElevation == nextElevation) {
        return;
    }

    /* Transfer from old to next elevation without deleting Entity from heap*/

    /* Deregister from old elevation */
    oldElevation->deregisterEntityOrThrow(entity);

    /* Register in next elevation */
    nextElevation->registetedEntityOrThrow(entity);
}


/* Creation/Deletion */

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


void World::deleteElevationOrThrow(Elevation* elevation) {

}