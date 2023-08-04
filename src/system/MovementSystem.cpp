#include "MovementSystem.h"
#include "MovementComponent.h"

void MovementSystem::init() {

}


void MovementSystem::update(Entity *entity, float dt) {
    auto cmp = entity->getComponent<MovementComponent>();
    if(cmp) {
        cmp->update(dt);
    }
}