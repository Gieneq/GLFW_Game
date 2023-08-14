#include "Camera.h"
#include "Entity.h"


void Camera::update(float dt) {
    if(focusedEntity != nullptr) {
        auto focusedWorldPosition = focusedEntity->getCuboidWorldSpace().topLeft;
        position.x = focusedWorldPosition.x;
        position.y = focusedWorldPosition.x;
        position.z = focusedWorldPosition.x;
    }
}

void Camera::focusOn(Entity* e) {
    focusedEntity = e;
}