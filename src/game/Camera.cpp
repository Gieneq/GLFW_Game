#include "Camera.h"
#include "Entity.h"


void Camera::update(float dt) {
    if(focusedEntity != nullptr) {
        position.set(focusedEntity->getCuboid().toWorldSpace().value().topLeftBottom());
    }
}

void Camera::focusOn(Entity* e) {
    focusedEntity = e;
}