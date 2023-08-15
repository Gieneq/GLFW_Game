#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"
#include <optional>

class LocationComponent;
class MovementComponent;
class CollisionComponent;
class World;
class Elevation {
public:
    Elevation() = default;
    Elevation(int elevation, World* containingWorld) : elevation(elevation), containingWorld(containingWorld) {}

    inline World* getContainingWorld() const {
        return containingWorld;
    }

    inline int getIndex() const {
        return elevation;
    }

    inline float getWorldSpaceZ() const {
        return static_cast<float>(elevation);
    }

    inline int getTotalEntitiesCount() const {
        return static_cast<int>(floorEntities.size() + clutterEntities.size() 
            + staticEntities.size() + dynamicEntities.size());
    }

    inline const std::vector<CollisionComponent*>& getCollisionComponents() const {
        return collisionComponentsRegister;
    }

    inline const std::vector<MovementComponent*>& getMovementComponents() const {
        return movementComponentsRegister;
    }

    const std::vector<Entity*>& getFloorEntities() const {
        return floorEntities;
    }

    const std::vector<Entity*>& getClutterEntities() const {
        return clutterEntities;
    }

    const std::vector<Entity*>& getStaticEntities() const {
        return staticEntities;
    }

    const std::vector<Entity*>& getDynamicEntities() const {
        return dynamicEntities;
    }

    const std::vector<Entity*>& getBiggerEntities() const {
        return biggerEntitiesRegister;
    }

private:
    int elevation{0};
    void addEntitisComponentsToRegisters(Entity* e);
    
    /* Only removes data from registers. Dispose of Entity is made separately in World */
    bool deleteEntity(Entity* e);
    
    void addFloorEntity(Entity* e);
    
    void addClutterEntity(Entity* e);

    void addStaticEntity(Entity* e);

    void addDynamicEntity(Entity* e);

    World* containingWorld{nullptr};
    
    // todo - I see use of Enum and templating here
    std::vector<Entity*> floorEntities;
    std::vector<Entity*> clutterEntities;
    std::vector<Entity*> staticEntities;
    std::vector<Entity*> dynamicEntities;

    std::vector<Entity*> biggerEntitiesRegister;
    std::vector<CollisionComponent*> collisionComponentsRegister;
    std::vector<MovementComponent*> movementComponentsRegister;

    /* World manages elevation indices */
    friend class World;
    friend class Loader;
    friend class Entity;
};


/**
 * World is responsible for managing elevations and entities.
 * 
*/
class World {
public:
    // todo dispose entities in destructor

    Elevation* appendElevation();

    std::optional<Elevation*> getElevation(int elevation);

    inline std::vector<Elevation>& getAllElevations() {
        return elevations;
    }

    std::optional<Elevation*> getTopElevation();

    int getElevationsCount() const;

    bool moveDynamicEntityToElevation(Entity* e, int destinationElevationIndex);

    // todo - I see use of Enum and templating here
    std::optional<Entity *> createFloorEntity(int elevation);

    std::optional<Entity *> createFloorEntity(Elevation* elevation);

    std::optional<Entity *> createClutterEntity(int elevation);

    std::optional<Entity *> createClutterEntity(Elevation* elevation);

    std::optional<Entity *> createStaticEntity(int elevation);

    std::optional<Entity *> createStaticEntity(Elevation* elevation);

    std::optional<Entity *> createDynamicEntity(int elevation);

    std::optional<Entity *> createDynamicEntity(Elevation* elevation);

    bool deleteEntity(Entity* e);
    
    Player player;

private:
    std::vector<Elevation> elevations;
    std::vector<Entity *> allEntities;
};
