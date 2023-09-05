#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <typeinfo>

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
    Elevation(int elevation, World& containingWorld) : elevation(elevation), containingWorld(containingWorld) {}

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
    template <EntityType type>
    int getEntitiesCount() const {
        return static_cast<int>(entitiesRegisterByType.at(type).size());
    }

    template <typename Cmp>
    int getComponentsCount() const {
        return static_cast<int>(componentsRegisterByType.at(ComponentName<Cmp>).size());
    }

    /* Access elements & iterators */
    template <EntityType type>
    Entity& getEntityOrThrow(int index) {
        return *entitiesRegisterByType.at(type).at(index);
    }

    template <EntityType type>
    const Entity& getEntityOrThrow(int index) const {
        return *entitiesRegisterByType.at(type).at(index);
    }

    template <EntityType type>
    std::vector<Entity*>::iterator entitiesBegin() {
        return entitiesRegisterByType.at(type).begin();
    }

    template <EntityType type>
    std::vector<Entity*>::iterator entitiesEnd() {
        return entitiesRegisterByType.at(type).end();
    }

    template <EntityType type>
    std::vector<Entity*>::const_iterator entitiesBegin() const {
        return entitiesRegisterByType.at(type).cbegin();
    }

    template <EntityType type>
    std::vector<Entity*>::const_iterator entitiesEnd() const {
        return entitiesRegisterByType.at(type).cend();
    }

    
    /* Other */

    template <EntityType type>
    Entity* getEntityByXY(const Vect2F& elevationSpacePoint) {
        /* Fix preventing hitting edge */
        const Vect2F consistentPoint{elevationSpacePoint.x + EDGE_EPSILON, elevationSpacePoint.y + EDGE_EPSILON};

        for(auto entity : entitiesRegisterByType.at(type)) {
            if(entity->getCuboid().value().getFlatten().hasPointInside(consistentPoint)) {
                return entity;
            }
        }
        return nullptr;
    }

    template <EntityType type>
    EntitySegment3X3<type> getEntities3X3(const Vect2F& centerElevationSpacePoint, const Size2F tileSize) {
        EntitySegment3X3<type> result;

        /* Fix preventing hitting edge */
        const Vect2F consistentPoint{centerElevationSpacePoint.x + EDGE_EPSILON, centerElevationSpacePoint.y + EDGE_EPSILON};

        for(int row = -1; row < 2; ++row) {
            for(int col = -1; col < 2; ++col) {
                const Vect2F entityCenter{consistentPoint.x + tileSize.w * col, consistentPoint.y + tileSize.h * row};
                result.setRelative(std::make_optional(getEntityByXY<type>(entityCenter)), col, row);
            }
        }

        return result;
    }

    template <EntityType type>
    std::vector<Entity*> getEntitiesIntersetingWith(const Rect4F& rectElevationSpace) {
        std::vector<Entity*> result;

        for(auto entity : entitiesRegisterByType.at(type)) {
            if(entity->getCuboid().value().getFlatten().checkIntersection(rectElevationSpace)) {
                result.push_back(entity);
            }
        }

        return result;
    }

private:
    int elevation{0};
    void addEntitisComponentsToRegisters(Entity* entity); //Used by Entity

    void moveEntityToElevationOrThrow(Entity* entity, Elevation* nextElevation); //Used by World
    
    //Used by World
    template <EntityType type> 
    Entity* createEntityOrThrow() {
        /* Add entity to proper container */
        Entity* newEntity = nullptr;
        try {
            newEntity = new Entity(this, type);
        } catch(std::bad_alloc& e) {
            std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
            throw e;
        }

        try {
            registetedEntityOrThrow<type>(newEntity);
        } catch(std::invalid_argument& e) {
            std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
            throw std::bad_alloc();
        }

        return newEntity;
    }

    template <EntityType type>
    void registetedEntityOrThrow(Entity* newEntity) {
        /* Type cannot be ALL */
        if(type == EntityType::ALL) {
            throw std::invalid_argument("Elevation::registetedEntityOrThrow: Type cannot be ALL");
        }
        
        entitiesRegisterByType.at(type).push_back(newEntity);

        entitiesRegisterByType.at(EntityType::ALL).push_back(newEntity);
    }

    template <EntityType type>
    void deregisterEntityOrThrow(Entity* entity) {
        
    }

    bool deleteEntityIfExists(Entity* entity); //Used by World

    World& containingWorld;
    
    std::vector<Entity*> entitiesContainer;
    std::unordered_map<EntityType, std::vector<Entity*>> entitiesRegisterByType {
        {EntityType::NONE, {}},
        {EntityType::FLOOR, {}},
        {EntityType::CLUTTER, {}},
        {EntityType::STATIC, {}},
        {EntityType::DYNAMIC, {}},
        {EntityType::BORDER, {}},
        {EntityType::BIGGER, {}},
        {EntityType::ALL, {}},
    };

    std::unordered_map<std::string, std::vector<Component*>> componentsRegisterByTypeName {
        // {ComponentName<MovementComponent>::get(), {}},
        // {ComponentName<CollisionComponent>::get(), {}},
        {"MovementComponent", {}},
        {"CollisionComponent", {}},
    };

    /* World manages elevation indices */
    friend class World;
    friend class Loader;
    friend class Entity;
};

