#include "World.h"
#include <iostream>
#include "CollisionComponents.h"
#include "MovementComponent.h"
#include "LocationComponent.h"

void Floor::addEntitysComponentsToRegisters(Entity* e) {
    CollisionComponent* cc = e->getComponent<CollisionComponent>();
    if(cc) {
        collisionComponentsRegister.push_back(cc);
    }
    MovementComponent* mc = e->getComponent<MovementComponent>();
    if(mc) {
        movementComponentsRegister.push_back(mc);
    }
    LocationComponent* lc = e->getComponent<LocationComponent>();
    if(lc) {
        locationComponentsRegister.push_back(lc);
    }
}

void Floor::addFloorEntity(Entity* e) {
    floorEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}

void Floor::addClutterEntity(Entity* e) {
    clutterEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}

void Floor::addStaticEntity(Entity* e) {
    staticEntities.push_back(e);
    biggerEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}

void Floor::addDynamicEntity(Entity* e) {
    dynamicEntities.push_back(e);
    biggerEntities.push_back(e);
    addEntitysComponentsToRegisters(e);
}