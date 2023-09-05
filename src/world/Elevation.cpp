#include "Elevation.h"
#include <iostream>
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include <algorithm>


void Elevation::addEntitisComponentsToRegisters(Entity* entity) {
    CollisionComponent* cc = entity->getComponent<CollisionComponent>();
    if(cc) {
        componentsRegisterByTypeName.at("CollisionComponent").push_back(cc);
    }
    MovementComponent* mc = entity->getComponent<MovementComponent>();
    if(mc) {
        componentsRegisterByTypeName.at("MovementComponent").push_back(mc);
    }
}

void Elevation::moveEntityToElevationOrThrow(Entity* entity, Elevation* nextElevation) {
    /* Check if entity is in this elevation */
    auto entityIt = std::find(allEntitiesRegister.begin(), allEntitiesRegister.end(), entity);
    if(entityIt == allEntitiesRegister.end()) {
        std::cerr << "Elevation::moveEntityToElevationOrThrow: Entity not found in this elevation" << std::endl;
        throw std::invalid_argument("Elevation::moveEntityToElevationOrThrow: Entity not found in this elevation");
    }

    /* Deregister from current elevation */
    deregisterEntityOrThrow(entity);

    /* Register in next elevation */
    nextElevation->registetedEntityOrThrow(entity);
    std::cout << "Entity moved to elevation " << nextElevation->getIndex() << std::endl;
}

// Entity* Elevation::createEntityOrThrow(EntityType type) {
//     /* Add entity to proper container */
//     Entity* newEntity = nullptr;
//     try {
//         newEntity = new Entity(this, type);
//     } catch(std::bad_alloc& e) {
//         std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
//         throw e;
//     }

//     try {
//         registetedEntityOrThrow(newEntity);
//     } catch(std::invalid_argument& e) {
//         std::cerr << "Elevation::createEntityOrThrow: " << e.what() << std::endl;
//         throw std::bad_alloc();
//     }

//     return newEntity;
// }

void Elevation::registetedEntityOrThrow(Entity* newEntity) {
    /* Check if entity already registered in world */
    auto entityIt = std::find(containingWorld.allEntitiesRegisterBegin(), containingWorld.allEntitiesRegisterEnd(), newEntity);
    if(entityIt != containingWorld.allEntitiesRegisterEnd()) {
        std::cerr << "Elevation::addEntity: Entity already registered in world" << std::endl;
        throw std::invalid_argument("Elevation::addEntity: Entity already registered in world");
    }

    /* Assign to specific group */
    switch(newEntity->getType()) {
        case EntityType::FLOOR:
            floorEntities.push_back(newEntity);
            break;
        case EntityType::CLUTTER:
            clutterEntities.push_back(newEntity);
            break;
        case EntityType::STATIC:
            staticEntities.push_back(newEntity);
            biggerEntitiesRegister.push_back(newEntity);
            break;
        case EntityType::DYNAMIC:
            dynamicEntities.push_back(newEntity);
            biggerEntitiesRegister.push_back(newEntity);
            break;
        default:
            std::cout << "Elevation::addEntity: Unknown entity type" << std::endl;
            throw std::invalid_argument("Elevation::addEntity: Unknown entity type");
            break;
    }

    addEntitisComponentsToRegisters(newEntity);
    
    /* Finally add to all elevations entities register */
    allEntitiesRegister.push_back(newEntity);

    newEntity->setElevation(this);
}

void Elevation::deregisterEntityOrThrow(Entity* entity) {
    /* Find entity in any container */
    auto floorEntityIt = std::find(floorEntities.begin(), floorEntities.end(), entity);
    auto clutterEntityIt = std::find(clutterEntities.begin(), clutterEntities.end(), entity);
    auto staticEntityIt = std::find(staticEntities.begin(), staticEntities.end(), entity);
    auto dynamicEntityIt = std::find(dynamicEntities.begin(), dynamicEntities.end(), entity);

    /* If entity not found in any container, return false */
    if(floorEntityIt == floorEntities.end() 
        && clutterEntityIt == clutterEntities.end() 
        && staticEntityIt == staticEntities.end() 
        && dynamicEntityIt == dynamicEntities.end()) {
        std::cerr << "Elevation::deregisterEntityOrThrow: Entity not found in any container" << std::endl;
        throw std::invalid_argument("Elevation::deregisterEntityOrThrow: Entity not found in any container");
    }

    /* Remove entity from registers */
    auto biggerEntityIt = std::find(biggerEntitiesRegister.begin(), biggerEntitiesRegister.end(), entity);
    if(biggerEntityIt != biggerEntitiesRegister.end()) {
        biggerEntitiesRegister.erase(biggerEntityIt);
    }

    auto allEntityIt = std::find(allEntitiesRegister.begin(), allEntitiesRegister.end(), entity);
    if(allEntityIt != allEntitiesRegister.end()) {
        allEntitiesRegister.erase(allEntityIt);
    }

    /* Remove entity's components from registers.
     * Find them by checking their parent entity's pointer 
    */
    
    /* CollisionComponent */
    auto collisionCmpFindResult = std::find_if(collisionComponentsRegister.begin(), collisionComponentsRegister.end(), 
        [entity](CollisionComponent* cc) {
            return cc->getParentEntity() == entity;
        });
    
    if(collisionCmpFindResult != collisionComponentsRegister.end()) {
        collisionComponentsRegister.erase(collisionCmpFindResult);
    }

    /* MovementComponent */
    auto movementCmpFindResult = std::find_if(movementComponentsRegister.begin(), movementComponentsRegister.end(), 
        [entity](MovementComponent* mc) {
            return mc->getParentEntity() == entity;
        });

    if(movementCmpFindResult != movementComponentsRegister.end()) {
        movementComponentsRegister.erase(movementCmpFindResult);
    }


    /* Remove entity from container */
    if(floorEntityIt != floorEntities.end()) {
        floorEntities.erase(floorEntityIt);
    }
    if(clutterEntityIt != clutterEntities.end()) {
        clutterEntities.erase(clutterEntityIt);
    }
    if(staticEntityIt != staticEntities.end()) {
        staticEntities.erase(staticEntityIt);
    }
    if(dynamicEntityIt != dynamicEntities.end()) {
        dynamicEntities.erase(dynamicEntityIt);
    }

    entity->setElevation(nullptr);
}

bool Elevation::deleteEntityIfExists(Entity* entity) {
    try {
        deregisterEntityOrThrow(entity);
    } catch(std::invalid_argument& e) {
        std::cerr << "Elevation::deleteEntityIfExists: " << e.what() << std::endl;
        return false;
    }
    /*
     * Seems entity registers and all its component are removed.
        * Now delete entity.
    */
    delete entity;

    return true;
}


/* Other */

// Entity* Elevation::getFloorEntityByXY(const Vect2F& elevationSpacePoint) {

//     /* Fix preventing hitting edge */
//     const Vect2F consistentPoint{elevationSpacePoint.x + EDGE_EPSILON, elevationSpacePoint.y + EDGE_EPSILON};

//     for(auto entity : floorEntities) {
//         if(entity->getCuboid().value().getFlatten().hasPointInside(consistentPoint)) {
//             return entity;
//         }
//     }
//     return nullptr;
// }

// FloorSegment3X3 Elevation::getFloorEntities3X3(const Vect2F& centerElevationSpacePoint, const Size2F tileSize) {
//     FloorSegment3X3 result;

//     /* Fix preventing hitting edge */
//     const Vect2F consistentPoint{centerElevationSpacePoint.x + EDGE_EPSILON, centerElevationSpacePoint.y + EDGE_EPSILON};

//     for(int row = -1; row < 2; ++row) {
//         for(int col = -1; col < 2; ++col) {
//             const Vect2F tileCenter{consistentPoint.x + tileSize.w * col, consistentPoint.y + tileSize.h * row};
//             result.setRelative(std::make_optional(getFloorEntityByXY(tileCenter)), col, row);
//         }
//     }

//     return result;
// }


// std::vector<Entity*> Elevation::getFloorEntitiesInRect(const Rect4F& rectElevationSpace) {
//     std::vector<Entity*> result;

//     for(auto entity : floorEntities) {
//         if(entity->getCuboid().value().getFlatten().checkIntersection(rectElevationSpace)) {
//             result.push_back(entity);
//         }
//     }

//     return result;
// }

// std::vector<CollisionComponent*> Elevation::getCollisionComponentsInRect(const Rect4F& flattenCollisionDetectorElevation) {
//     std::vector<CollisionComponent*> result;

//     for(auto collisonCmp : collisionComponentsRegister) {
//         if(collisonCmp->getParentEntity()->getCuboid().value().getFlatten().checkIntersection(flattenCollisionDetectorElevation)) {
//             result.push_back(collisonCmp);
//         }
//     }

//     return result;
// }

// std::vector<Entity*> Elevation::getAnyIntersectingEntities(Vect2F pointElevationSpace) {
//     std::vector<Entity*> result;

//     std::copy_if(allEntitiesRegister.begin(), allEntitiesRegister.end(), std::back_inserter(result), 
//         [pointElevationSpace](Entity* e) {
//             return e->getCuboid().value().getFlatten().hasPointInside(pointElevationSpace);
//         });

//     return result;
// }

// std::vector<Entity*> Elevation::getAnyIntersectingEntities(const Rect4F& rectElevationSpace, Vect2F direction) {
//     /* Find 2 vertices pointer by direction */
//     Vect2F firstVertex;
//     Vect2F secondVertex;
//     if(direction.x > 0) {
//         firstVertex = rectElevationSpace.topRight();
//         secondVertex = rectElevationSpace.bottomRight();
//     } 
//     else if(direction.x < 0) {
//         firstVertex = rectElevationSpace.bottomLeft();
//         secondVertex = rectElevationSpace.topLeft();
//     } 
//     else if(direction.y < 0) {
//         firstVertex = rectElevationSpace.topLeft();
//         secondVertex = rectElevationSpace.topRight();
//     } 
//     else if(direction.y > 0) {
//         firstVertex = rectElevationSpace.bottomRight();
//         secondVertex = rectElevationSpace.bottomLeft();
//     }

//     //todo refactor

//     /* Find entities colliding with first vertex */
//     auto firstVertexCollidingEntities = getAnyIntersectingEntities(firstVertex);

//     /* Find entities colliding with second vertex */
//     auto secondVertexCollidingEntities = getAnyIntersectingEntities(secondVertex);

//     /* Combine results */
//     std::vector<Entity*> result;
    
//     /* Pass only unique */
//     for(Entity* e : firstVertexCollidingEntities) {
//         if(std::find(result.begin(), result.end(), e) == result.end()) {
//             result.push_back(e);
//         }
//     }

//     for(Entity* e : secondVertexCollidingEntities) {
//         if(std::find(result.begin(), result.end(), e) == result.end()) {
//             result.push_back(e);
//         }
//     }

//     return result;
// }

