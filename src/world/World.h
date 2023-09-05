#pragma once
#include "Elevation.h"
#include "Component.h"
#include <vector>
#include <unordered_map>
#include "Entity.h"
#include "Player.h"
#include "EntityContainers.h"
#include <optional>
#include <iostream>
#include <typeinfo>


/**
 * World is responsible for managing elevations and entities.
*/
class World {
public:
    ~World() = default;
    World() = default;

    Elevation* createElevationOrThrow();

    int getElevationsCount() const;

    /* Access Elevation */
    std::optional<Elevation*> getElevationOption(int elevation);

    std::optional<Elevation*> getLowerElevation(Elevation* elevation);
    
    std::optional<Elevation*> getHigherElevation(Elevation* elevation);

    std::optional<Elevation*> getTopElevationOption();

    Elevation& operator[](int elevation);

    const Elevation& operator[](int elevation) const;

    Elevation& getTopElevationOrThrow();

    const Elevation& getTopElevationConstOrThrow();

    /* Iterators over Elevations */

    std::vector<Elevation*>::iterator begin();

    std::vector<Elevation*>::iterator end();

    std::vector<Elevation*>::const_iterator begin() const;

    std::vector<Elevation*>::const_iterator end() const;

    /* Other */
    void moveDynamicEntityToElevationOrThrow(Entity* e, int destinationElevationIndex);

    /* Creation Entities on Elevations */
    
    Entity* createEntityOnElevationOrThrow(int elevationIndex, EntityType type);

    Entity* createEntityOnElevationOrThrow(Elevation* elevation, EntityType type);

    /* Deletion */
    void deleteEntityOrThrow(Entity* e);
    
    std::vector<Entity*> getAnyIntersectingEntities(Vect2F pointElevationSpace, int elevationIndex);
    
    Player player;

private:
    std::vector<Elevation*> elevations;
};
