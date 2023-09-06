#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <typeinfo>
#include <type_traits>

#include "Component.h"
#include "Maths.h"
#include "Coordinates.h"
#include "Entity.h"
#include "EntityContainers.h"

#define EDGE_EPSILON 0.0001f

class MovementComponent;
class CollisionComponent;
class World;
class ElevationDepth;

class ElevationDepth {
public:
    virtual ~ElevationDepth() = default;

    virtual float z() const = 0;

    virtual int getIndex() const = 0;

protected:
    virtual void setIndex(const int index) = 0;
};


class Elevation : public ElevationDepth {
    ~Elevation() = default;
    Elevation(int elevation, World& containingWorld) : elevation(elevation), containingWorld(containingWorld) {

        /* Setup regiester for entities */
        for(auto type : Entity::AllTypes) {
            entitiesRegisterByType.insert({type, {}});
        }

        /* Setup regiester for components */
        //todo
    }

public:
    inline World& getContainingWorld() {
        return containingWorld;
    }

    inline const World& getContainingWorld() const {
        return containingWorld;
    }

    virtual int getIndex() const {
        return elevation;
    }

    virtual void setIndex(const int index) {
        this->elevation = index;
    }

    virtual float z() const {
        return static_cast<float>(elevation);
    }

    inline Cuboid6F elevationToWorldSpace(const Cuboid6F& elevationSpaceCuboid) const {
        return elevationSpaceCuboid.getTranslated(Vect3F{0, 0, this->z()});
    }


    /* Counts */

    inline int getEntitiesCount(Entity::Type type) const {
        return static_cast<int>(entitiesRegisterByType.at(type).size());
    }

    inline int getAllEntitiesCount() const {
        return static_cast<int>(allEntities.size());
    }

    inline int getCollisionComponentsCount() const {
        return static_cast<int>(collisionComponentsRegister.size());
    }

    inline int getMovementComponentsCount() const {
        return static_cast<int>(movementComponentsRegister.size());
    }

    inline int getBiggerEntitiesCount() {
        return static_cast<int>(biggerEntitiesRegister.size());
    }


    /* Access elements & iterators - entity by type */

    inline Entity& getEntityOrThrow(Entity::Type type, int index) {
        return *entitiesRegisterByType.at(type).at(index);
    }

    inline const Entity& getEntityOrThrow(Entity::Type type, int index) const {
        return *entitiesRegisterByType.at(type).at(index);
    }

    inline std::vector<Entity*>::iterator entitiesBegin(Entity::Type type) {
        return entitiesRegisterByType.at(type).begin();
    }

    inline std::vector<Entity*>::iterator entitiesEnd(Entity::Type type) {
        return entitiesRegisterByType.at(type).end();
    }

    inline std::vector<Entity*>::const_iterator entitiesBegin(Entity::Type type) const {
        return entitiesRegisterByType.at(type).cbegin();
    }

    inline std::vector<Entity*>::const_iterator entitiesEnd(Entity::Type type) const {
        return entitiesRegisterByType.at(type).cend();
    }


    /* Access elements & iterators - entity all */

    Entity* operator[](int index) {
        return allEntities[index];
    }

    const Entity* operator[](int index) const {
        return allEntities[index];
    }

    inline std::vector<Entity*>::iterator begin(Entity::Type type) {
        return allEntities.begin();
    }

    inline std::vector<Entity*>::iterator end(Entity::Type type) {
        return allEntities.end();
    }

    inline std::vector<Entity*>::const_iterator begin(Entity::Type type) const {
        return allEntities.cbegin();
    }

    inline std::vector<Entity*>::const_iterator end(Entity::Type type) const {
        return allEntities.cend();
    }


    /* Bigger entities */

    inline std::vector<Entity*>::iterator entitiesBiggerBegin() {
        return biggerEntitiesRegister.begin();
    }

    inline std::vector<Entity*>::iterator entitiesBiggerEnd() {
        return biggerEntitiesRegister.end();
    }

    inline std::vector<Entity*>::const_iterator entitiesBiggerBegin() const {
        return biggerEntitiesRegister.cbegin();
    }

    inline std::vector<Entity*>::const_iterator entitiesBiggerEnd() const {
        return biggerEntitiesRegister.cend();
    }


    /* Access elements & iterators - components registers */

    /* Collision - todo combine */

    inline std::vector<CollisionComponent*>::iterator collisionComponentsBegin() {
        return collisionComponentsRegister.begin();
    }

    inline std::vector<CollisionComponent*>::iterator collisionComponentsEnd() {
        return collisionComponentsRegister.end();
    }

    inline std::vector<CollisionComponent*>::const_iterator collisionComponentsBegin() const {
        return collisionComponentsRegister.cbegin();
    }

    inline std::vector<CollisionComponent*>::const_iterator collisionComponentsEnd() const {
        return collisionComponentsRegister.cend();
    }


    /* Movement - todo combine */

    inline std::vector<MovementComponent*>::iterator movementComponentsBegin() {
        return movementComponentsRegister.begin();
    }

    inline std::vector<MovementComponent*>::iterator movementComponentsEnd() {
        return movementComponentsRegister.end();
    }

    inline std::vector<MovementComponent*>::const_iterator movementComponentsBegin() const {
        return movementComponentsRegister.cbegin();
    }

    inline std::vector<MovementComponent*>::const_iterator movementComponentsEnd() const {
        return movementComponentsRegister.cend();
    }


    /* Other */

    Entity* getEntityByXY(Entity::Type type, const Vect2F& elevationSpacePoint);

    EntitySegment3X3 getEntities3X3(Entity::Type type, 
        const Vect2F& centerElevationSpacePoint, const Size2F tileSize);

    std::vector<Entity*> getEntitiesIntersetingWith(Entity::Type type, 
        const Rect4F& rectElevationSpace);

protected:
    /* Entity Create/Delete */

    Entity* createEntityOrThrow(Entity::Type type);
    
    bool deleteEntityIfExists(Entity* oldEntity);

private:
    /* Registartion of Entity or its Components */

    void registetedEntityOrThrow(Entity* newEntity);
    
    void regiesterComponentsOfEntity(Entity* entity);


    /* Deregistartion of Entity or its Components */

    void deregisterEntityOrThrow(Entity* oldEntity);

    void deregiesterComponentsOfEntity(Entity* entity);
    

    /* Member variables */
    int elevation{0};

    World& containingWorld;
    
    std::vector<Entity*> allEntities;

    std::vector<Entity*> biggerEntitiesRegister;

    std::unordered_map<Entity::Type, std::vector<Entity*>> entitiesRegisterByType;

    std::vector<CollisionComponent*> collisionComponentsRegister;

    std::vector<MovementComponent*> movementComponentsRegister;

    /* World manages elevation indices */
    friend class World;
    friend class Loader;
    friend class Entity;
};

