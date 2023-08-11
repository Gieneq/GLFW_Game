#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"

class LocationComponent;
class MovementComponent;
class CollisionComponent;
class World;
class Floor {
public:
    Floor() = default;
    Floor(int elevation, World* containingWorld) : elevation(elevation), containingWorld(containingWorld) {}

    int elevation{0};
    
    World* getContainingWorld() const {
        return containingWorld;
    }

    float getZ() const {
        return static_cast<float>(elevation);
    }

    int getTotalEntitiesCount() const {
        return static_cast<int>(floorEntities.size() + clutterEntities.size() 
            + staticEntities.size() + dynamicEntities.size());
    }

    const std::vector<CollisionComponent*>& getCollisionComponents() const {
        return collisionComponentsRegister;
    }

    const std::vector<MovementComponent*>& getMovementComponents() const {
        return movementComponentsRegister;
    }

    const std::vector<LocationComponent*>& getLocationComponents() const {
        return locationComponentsRegister;
    }

    void addFloorEntity(Entity* e);
    void addClutterEntity(Entity* e);
    void addStaticEntity(Entity* e);
    void addDynamicEntity(Entity* e);

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
        return biggerEntities;
    }

    bool removeEntity(Entity* e);

private:
    void addEntitysComponentsToRegisters(Entity* e);

    World* containingWorld{nullptr};
    std::vector<Entity*> floorEntities;
    std::vector<Entity*> clutterEntities;
    std::vector<Entity*> staticEntities;
    std::vector<Entity*> dynamicEntities;
    std::vector<Entity*> biggerEntities;
    std::vector<CollisionComponent*> collisionComponentsRegister;
    std::vector<MovementComponent*> movementComponentsRegister;
    std::vector<LocationComponent*> locationComponentsRegister;
};

class World {
public:
    void appendFloor(int elevation);
    std::optional<Floor*> getFloor(int elevation);
    std::optional<Floor*> getTopFloor();
    int getFloorsCount() const;
    bool moveEntityToFloor(Entity* e, int newElevation);
    
    Player player;

private:
    std::vector<Floor> floors;
};
