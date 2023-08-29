#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"
#include <optional>
#include <iostream>

class LocationComponent;
class MovementComponent;
class CollisionComponent;
class World;

class TilesPair {
public:
    TilesPair(Entity* left, Entity* right) : left(left), right(right) {}
    bool hasAny() const {
        return left != nullptr || right != nullptr;
    }

    Rect4F getBoundingRect() const;

    Entity* left;
    Entity* right;

    friend std::ostream& operator<<(std::ostream& os, const TilesPair& tilesPair) {
        os << "[L: "  << (tilesPair.left ? tilesPair.left->getId() : -2);
        os << ", R: "  << (tilesPair.right ? tilesPair.right->getId() : -2);
        os << "]";
        return os;
    }
};

class TilesQuad {
public:
    TilesQuad(Entity* topLeftGlobal, Entity* topRightGlobal, Entity* bottomLeftGlobal, Entity* bottomRightGlobal) 
        : topLeftGlobal(topLeftGlobal), topRightGlobal(topRightGlobal), bottomLeftGlobal(bottomLeftGlobal), bottomRightGlobal(bottomRightGlobal) {}

    TilesPair getFrontTilesRelativeToDirection(Vect2F direction) const;

    bool hasAnyTile() const {
        return topLeftGlobal != nullptr || topRightGlobal != nullptr || bottomLeftGlobal != nullptr || bottomRightGlobal != nullptr;
    }    

    bool hasMissingTile() const {
        return topLeftGlobal == nullptr || topRightGlobal == nullptr || bottomLeftGlobal == nullptr || bottomRightGlobal == nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const TilesQuad& tilesQuad) {
        os << "[TL: "  << (tilesQuad.topLeftGlobal ? tilesQuad.topLeftGlobal->getId() : -2);
        os << ", TR: "  << (tilesQuad.topRightGlobal ? tilesQuad.topRightGlobal->getId() : -2);
        os << ", BL: "  << (tilesQuad.bottomLeftGlobal ? tilesQuad.bottomLeftGlobal->getId() : -2);
        os << ", BR: "  << (tilesQuad.bottomRightGlobal ? tilesQuad.bottomRightGlobal->getId() : -2);
        os << "]";
        return os;
    }

private:
    Entity* topLeftGlobal;
    Entity* topRightGlobal;
    Entity* bottomLeftGlobal;
    Entity* bottomRightGlobal;
};

class Elevation {
    Elevation(int elevation, World& containingWorld) : elevation(elevation), containingWorld(containingWorld) {}

public:
    inline World& getContainingWorld() {
        return containingWorld;
    }

    inline const World& getContainingWorldConst() const {
        return containingWorld;
    }

    inline int getIndex() const {
        return elevation;
    }

    inline float getWorldSpaceZ() const {
        return static_cast<float>(elevation);
    }

    Cuboid6F elevationToWorldSpace(const Cuboid6F& elevationSpaceCuboid) const;

    /* Counts */
    inline int getTotalEntitiesCount() const {
        return static_cast<int>(floorEntities.size() + clutterEntities.size() 
            + staticEntities.size() + dynamicEntities.size());
    }

    inline int getFloorEntitiesCount() const {
        return static_cast<int>(floorEntities.size());
    }

    inline int getClutterEntitiesCount() const {
        return static_cast<int>(clutterEntities.size());
    }

    inline int getStaticEntitiesCount() const {
        return static_cast<int>(staticEntities.size());
    }

    inline int getDynamicEntitiesCount() const {
        return static_cast<int>(dynamicEntities.size());
    }

    inline int getBiggerEntitiesCount() const {
        return static_cast<int>(biggerEntitiesRegister.size());
    }

    inline int getMovementComponentsCount() const {
        return static_cast<int>(movementComponentsRegister.size());
    }

    inline int getCollisionComponentsCount() const {
        return static_cast<int>(collisionComponentsRegister.size());
    }


    /* Access elements & iterators - Floor Entities */
    inline Entity& getFloorEntityOrThrow(int index) {
        return *floorEntities.at(index);
    }

    inline const Entity& getFloorEntityConstOrThrow(int index) const {
        return *floorEntities.at(index);
    }

    inline std::vector<Entity*>::iterator floorEntitiesBegin() {
        return floorEntities.begin();
    }

    inline std::vector<Entity*>::iterator floorEntitiesEnd() {
        return floorEntities.end();
    }

    inline std::vector<Entity*>::const_iterator floorEntitiesBegin() const {
        return floorEntities.begin();
    }

    inline std::vector<Entity*>::const_iterator floorEntitiesEnd() const {
        return floorEntities.end();
    }

    
    /* Access elements & iterators - Clutter Entities */
    inline Entity& getClutterEntityOrThrow(int index) {
        return *clutterEntities.at(index);
    }

    inline const Entity& getClutterEntityConstOrThrow(int index) const {
        return *clutterEntities.at(index);
    }

    inline std::vector<Entity*>::iterator clutterEntitiesBegin() {
        return clutterEntities.begin();
    }

    inline std::vector<Entity*>::iterator clutterEntitiesEnd() {
        return clutterEntities.end();
    }

    inline std::vector<Entity*>::const_iterator clutterEntitiesBegin() const {
        return clutterEntities.begin();
    }

    inline std::vector<Entity*>::const_iterator clutterEntitiesEnd() const {
        return clutterEntities.end();
    }


    /* Access elements & iterators - Static Entities */
    inline Entity& getStaticEntityOrThrow(int index) {
        return *staticEntities.at(index);
    }

    inline const Entity& getStaticEntityConstOrThrow(int index) const {
        return *staticEntities.at(index);
    }

    inline std::vector<Entity*>::iterator staticEntitiesBegin() {
        return staticEntities.begin();
    }

    inline std::vector<Entity*>::iterator staticEntitiesEnd() {
        return staticEntities.end();
    }

    inline std::vector<Entity*>::const_iterator staticEntitiesBegin() const {
        return staticEntities.begin();
    }

    inline std::vector<Entity*>::const_iterator staticEntitiesEnd() const {
        return staticEntities.end();
    }


    /* Access elements & iterators - Dynamic Entities */
    inline Entity& getDynamicEntityOrThrow(int index) {
        return *dynamicEntities.at(index);
    }

    inline const Entity& getDynamicEntityConstOrThrow(int index) const {
        return *dynamicEntities.at(index);
    }

    inline std::vector<Entity*>::iterator dynamicEntitiesBegin() {
        return dynamicEntities.begin();
    }

    inline std::vector<Entity*>::iterator dynamicEntitiesEnd() {
        return dynamicEntities.end();
    }

    inline std::vector<Entity*>::const_iterator dynamicEntitiesBegin() const {
        return dynamicEntities.begin();
    }

    inline std::vector<Entity*>::const_iterator dynamicEntitiesEnd() const {
        return dynamicEntities.end();
    }

    
    /* Iterators - Bigger Entities Register */
    inline std::vector<Entity*>::iterator biggerEntitiesRegisterBegin() {
        return biggerEntitiesRegister.begin();
    }

    inline std::vector<Entity*>::iterator biggerEntitiesRegisterEnd() {
        return biggerEntitiesRegister.end();
    }

    inline std::vector<Entity*>::const_iterator biggerEntitiesRegisterBegin() const {
        return biggerEntitiesRegister.begin();
    }

    inline std::vector<Entity*>::const_iterator biggerEntitiesRegisterEnd() const {
        return biggerEntitiesRegister.end();
    }


    /* Iterators - All Entities Register */
    inline std::vector<Entity*>::iterator allEntitiesRegisterBegin() {
        return allEntitiesRegister.begin();
    }

    inline std::vector<Entity*>::iterator allEntitiesRegisterEnd() {
        return allEntitiesRegister.end();
    }

    inline std::vector<Entity*>::const_iterator allEntitiesRegisterBegin() const {
        return allEntitiesRegister.begin();
    }

    inline std::vector<Entity*>::const_iterator allEntitiesRegisterEnd() const {
        return allEntitiesRegister.end();
    }


    /* Iterators - Collision Components Register */
    inline std::vector<CollisionComponent*>::iterator collisionComponentsRegisterBegin() {
        return collisionComponentsRegister.begin();
    }

    inline std::vector<CollisionComponent*>::iterator collisionComponentsRegisterEnd() {
        return collisionComponentsRegister.end();
    }

    inline std::vector<CollisionComponent*>::const_iterator collisionComponentsRegisterBegin() const {
        return collisionComponentsRegister.begin();
    }

    inline std::vector<CollisionComponent*>::const_iterator collisionComponentsRegisterEnd() const {
        return collisionComponentsRegister.end();
    }


    /* Iterators - Movement Components Register */
    inline std::vector<MovementComponent*>::iterator movementComponentsRegisterBegin() {
        return movementComponentsRegister.begin();
    }

    inline std::vector<MovementComponent*>::iterator movementComponentsRegisterEnd() {
        return movementComponentsRegister.end();
    }

    inline std::vector<MovementComponent*>::const_iterator movementComponentsRegisterBegin() const {
        return movementComponentsRegister.begin();
    }

    inline std::vector<MovementComponent*>::const_iterator movementComponentsRegisterEnd() const {
        return movementComponentsRegister.end();
    }
    
    /* Other */

    Entity* getFloorEntityByXY(const Vect2F& entityPosition);

    TilesQuad getFloorNearbyTilesQuad(const Rect4F& entityRect);

    std::vector<Entity*> getAnyCollidingEntities(Vect2F pointElevationSpace);

    std::vector<Entity*> getAnyCollidingEntities(const Rect4F& rectElevationSpace, Vect2F direction);

private:
    int elevation{0};
    void addEntitisComponentsToRegisters(Entity* entity); //Used by Entity

    void moveEntityToElevationOrThrow(Entity* entity, Elevation* nextElevation); //Used by World
    
    Entity* createEntityOrThrow(EntityType type); //Used by World
    void registetedEntityOrThrow(Entity* newEntity);
    void deregisterEntityOrThrow(Entity* entity);
    bool deleteEntityIfExists(Entity* entity); //Used by World

    World& containingWorld;
    
    std::vector<Entity*> floorEntities;
    std::vector<Entity*> clutterEntities;
    std::vector<Entity*> staticEntities;
    std::vector<Entity*> dynamicEntities;

    std::vector<Entity*> biggerEntitiesRegister;
    std::vector<Entity*> allEntitiesRegister;
    std::vector<CollisionComponent*> collisionComponentsRegister;
    std::vector<MovementComponent*> movementComponentsRegister;

    /* World manages elevation indices */
    friend class World;
    friend class Loader;
    friend class Entity;
};


/**
 * World is responsible for managing elevations and entities.
*/
class World {
public:
    // todo dispose entities in destructor
    World() = default;

    Elevation* createElevationOrThrow();

    int getElevationsCount() const;

    /* Access Elevation */
    std::optional<Elevation*> getElevationOption(int elevation);

    std::optional<Elevation*> getTopElevationOption();

    Elevation& operator[](int elevation);

    const Elevation& operator[](int elevation) const;

    Elevation& getTopElevationOrThrow();

    const Elevation& getTopElevationConstOrThrow();

    /* Iterators over Elevaions */

    std::vector<Elevation*>::iterator begin();

    std::vector<Elevation*>::iterator end();

    std::vector<Elevation*>::const_iterator begin() const;

    std::vector<Elevation*>::const_iterator end() const;

    /* Iterator over all Entities */
    inline std::vector<Entity*>::iterator allEntitiesRegisterBegin() {
        return allEntitiesRegister.begin();
    }

    inline std::vector<Entity*>::iterator allEntitiesRegisterEnd() {
        return allEntitiesRegister.end();
    }

    inline std::vector<Entity*>::const_iterator allEntitiesRegisterBegin() const {
        return allEntitiesRegister.begin();
    }

    inline std::vector<Entity*>::const_iterator allEntitiesRegisterEnd() const {
        return allEntitiesRegister.end();
    }

    /* Other */
    void moveDynamicEntityToElevationOrThrow(Entity* e, int destinationElevationIndex);// todo

    /* Creation Entities on Elevations */
    Entity* createEntityOnElevationOrThrow(int elevationIndex, EntityType type);

    Entity* createEntityOnElevationOrThrow(Elevation* elevation, EntityType type);

    /* Deletion */
    void deleteEntityOrThrow(Entity* e);
    
    std::vector<Entity*> getAnyCollidingEntities(Vect2F pointElevationSpace, int elevationIndex);
    
    Player player;

private:
    std::vector<Elevation*> elevations;
    std::vector<Entity*> allEntitiesRegister;
};
