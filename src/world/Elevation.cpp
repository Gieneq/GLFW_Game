#include "Elevation.h"
#include <iostream>
#include <algorithm>

#include "CollisionComponents.h"
#include "MovementComponent.h"
#include "Entity.h"

/* Entity Create/Delete */

Entity* Elevation::createEntityOrThrow(Entity::Type type) {
    /* Create new Entity on heap */
    Entity* newEntity = nullptr;
    try {
        newEntity = new Entity(this, type);
    } catch(std::bad_alloc& e) {
        std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
        throw e;
    }

    /* Register entity - add to proper register */
    try {
        registetedEntityOrThrow(newEntity);
    } catch(std::invalid_argument& e) {
        std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
        throw std::bad_alloc();
    }

    return newEntity;
}


bool Elevation::deleteEntityIfExists(Entity* oldEntity) {
    /* Remove all components from registers and entit from its register */
    try {
        deregisterEntityOrThrow(oldEntity);
    } catch(std::invalid_argument& e) {
        std::cerr << "Elevation::deleteEntityIfExists: " << e.what() << std::endl;
        return false;
    }

    /* Dispose from heap */
    delete oldEntity;
    return true;
}


/* Registartion of Entity or its Components */

void Elevation::registetedEntityOrThrow(Entity* newEntity) {
    /* Add to type register */
    const auto type = newEntity->getType();
    auto& typeRegister = entitiesRegisterByType.at(type);

    /* If already exist in type register, throw */
    if(std::find(typeRegister.begin(), typeRegister.end(), newEntity) != typeRegister.end()) {
        std::cerr << "Elevation::registetedEntityOrThrow: Entity already exist in type register" << std::endl;
        throw std::invalid_argument("Elevation::registetedEntityOrThrow: Entity already exist in type register");
    }

    /* If already exist in all register, throw */
    if(std::find(allEntities.begin(), allEntities.end(), newEntity) != allEntities.end()) {
        std::cerr << "Elevation::registetedEntityOrThrow: Entity already exist in all register" << std::endl;
        throw std::invalid_argument("Elevation::registetedEntityOrThrow: Entity already exist in all register");
    }

    /* If should be added but alredy registered in bigger entities */
    if(Entity::isBiggerType(type)) {
        if(std::find(biggerEntitiesRegister.begin(), biggerEntitiesRegister.end(), newEntity) != biggerEntitiesRegister.end()) {
            std::cerr << "Elevation::registetedEntityOrThrow: Entity already exist in bigger type register" << std::endl;
            throw std::invalid_argument("Elevation::registetedEntityOrThrow: Entity already exist in bigger type register");
        }
        biggerEntitiesRegister.push_back(newEntity);
    }

    /* Finally register */
    newEntity->setElevation(this);

    typeRegister.push_back(newEntity);

    allEntities.push_back(newEntity);

    /* Register all components */
    regiesterComponentsOfEntity(newEntity);
}

void Elevation::regiesterComponentsOfEntity(Entity* entity) {
    /* Add to collision components register */
    CollisionComponent* cc = entity->getComponent<CollisionComponent>();
    if(cc) {
        /* If not exist add */
        if(std::find(collisionComponentsRegister.begin(), collisionComponentsRegister.end(), cc) == collisionComponentsRegister.end()) {
            collisionComponentsRegister.push_back(cc);
        }
    }

    /* Add to movement components register */
    MovementComponent* mc = entity->getComponent<MovementComponent>();
    if(mc) {
        /* If not exist add */
        if(std::find(movementComponentsRegister.begin(), movementComponentsRegister.end(), mc) == movementComponentsRegister.end()) {
            movementComponentsRegister.push_back(mc);
        }
    }
}


/* Deregistartion of Entity or its Components */

void Elevation::deregisterEntityOrThrow(Entity* entity) {
    const auto type = entity->getType();
    auto& typeRegister = entitiesRegisterByType.at(type);

    /* Remove from type register */
    auto entityIt = std::find(typeRegister.begin(), typeRegister.end(), entity);
    if(entityIt == typeRegister.end()) {
        std::cerr << "Elevation::deregisterEntityOrThrow: Entity not found in type register" << std::endl;
        throw std::invalid_argument("Elevation::deregisterEntityOrThrow: Entity not found in type register");
    }

    /* Remove from all entities register */
    auto allEntityIt = std::find(allEntities.begin(), allEntities.end(), entity);
    if(allEntityIt == allEntities.end()) {
        std::cerr << "Elevation::deregisterEntityOrThrow: Entity not found in all entities register" << std::endl;
        throw std::invalid_argument("Elevation::deregisterEntityOrThrow: Entity not found in all entities register");
    }

    /* Remove from bigger if is actually bigger */
    if(Entity::isBiggerType(type)) {
        auto biggerEntityIt = std::find(biggerEntitiesRegister.begin(), biggerEntitiesRegister.end(), entity);
        if(biggerEntityIt == biggerEntitiesRegister.end()) {
            std::cerr << "Elevation::deregisterEntityOrThrow: Entity not found in bigger type register" << std::endl;
            throw std::invalid_argument("Elevation::deregisterEntityOrThrow: Entity not found in bigger type register");
        }
        biggerEntitiesRegister.erase(biggerEntityIt);
    }
    
    /* Finally deregister */
    entity->setElevation(nullptr);

    allEntities.erase(allEntityIt);

    typeRegister.erase(entityIt);

    /* Deregister all components */
    deregiesterComponentsOfEntity(entity);
}

void Elevation::deregiesterComponentsOfEntity(Entity* entity) {
    /* Remove from collision components register */
    CollisionComponent* cc = entity->getComponent<CollisionComponent>();
    if(cc) {
        /* If exists remove */
        auto ccIt = std::find(collisionComponentsRegister.begin(), collisionComponentsRegister.end(), cc);
        if(ccIt != collisionComponentsRegister.end()) {
            collisionComponentsRegister.erase(ccIt);
        }
    }

    /* Remove from movement components register */
    MovementComponent* mc = entity->getComponent<MovementComponent>();
    if(mc) {
        /* If exists remove */
        auto mcIt = std::find(movementComponentsRegister.begin(), movementComponentsRegister.end(), mc);
        if(mcIt != movementComponentsRegister.end()) {
            movementComponentsRegister.erase(mcIt);
        }
    }
}


/* Other */

Entity* Elevation::getEntityByXY(Entity::Type type, const Vect2F& elevationSpacePoint) {
    /* Fix preventing hitting edge */
    const Vect2F consistentPoint{elevationSpacePoint.x + EDGE_EPSILON, elevationSpacePoint.y + EDGE_EPSILON};

    for(auto entity : entitiesRegisterByType.at(type)) {
        if(entity->getCuboid().value().getFlatten().hasPointInside(consistentPoint)) {
            return entity;
        }
    }
    return nullptr;
}

EntitySegment3X3 Elevation::getEntities3X3(Entity::Type type, const Vect2F& centerElevationSpacePoint, const Size2F tileSize) {
    EntitySegment3X3 result{type};

    /* Fix preventing hitting edge */
    const Vect2F consistentPoint{centerElevationSpacePoint.x + EDGE_EPSILON, centerElevationSpacePoint.y + EDGE_EPSILON};

    for(int row = -1; row < 2; ++row) {
        for(int col = -1; col < 2; ++col) {
            const Vect2F entityCenter{consistentPoint.x + tileSize.w * col, consistentPoint.y + tileSize.h * row};
            result.setRelative(std::make_optional(getEntityByXY(type, entityCenter)), col, row);
        }
    }

    return result;
}

std::vector<Entity*> Elevation::getEntitiesIntersetingWith(Entity::Type type, const Rect4F& rectElevationSpace) {
    std::vector<Entity*> result;

    for(auto entity : entitiesRegisterByType.at(type)) {
        if(entity->getCuboid().value().getFlatten().checkIntersection(rectElevationSpace)) {
            result.push_back(entity);
        }
    }

    return result;
}

