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

    /* Counts */

    int getElevationsCount() const;


    /* Access Elevation */

    std::optional<Elevation*> getElevationOption(int elevation);
    
    std::optional<const Elevation*> getElevationOption(int elevation) const;

    std::optional<Elevation*> getLowerElevation(Elevation* elevation);
    
    std::optional<const Elevation*> getLowerElevation(Elevation* elevation) const;
    
    std::optional<Elevation*> getHigherElevation(Elevation* elevation);
    
    std::optional<const Elevation*> getHigherElevation(Elevation* elevation) const;

    std::optional<Elevation*> getTopElevationOption();
    
    std::optional<const Elevation*> getTopElevationOption() const;

    Elevation* operator[](int elevation);

    const Elevation* operator[](int elevation) const;


    /* Iterators over Elevations */

    inline std::vector<Elevation*>::iterator World::begin() {
        return elevations.begin();
    }

    inline std::vector<Elevation*>::iterator World::end() {
        return elevations.end();
    }

    inline std::vector<Elevation*>::const_iterator World::begin() const {
        return elevations.cbegin();
    }

    inline std::vector<Elevation*>::const_iterator World::end() const {
        return elevations.cend();
    }

    /* Other */
    void moveEntityToElevationOrThrow(Entity* e, int destinationElevationIndex);
    
    inline Player& getPlayer() {
        return player;
    }

    inline const Player& getPlayer() const {
        return player;
    }

private:
    /* Creation/Deletion */
    
    Elevation* createElevationOrThrow();

    void deleteElevationOrThrow(Elevation* elevation);


    /* Variables */
    
    Player player;

    std::vector<Elevation*> elevations;

    friend class Loader;
};
