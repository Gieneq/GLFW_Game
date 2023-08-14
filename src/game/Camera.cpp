#include "Camera.h"
#include "Entity.h"


void Camera::update(float dt) {
    if(focusedEntity != nullptr) {
        auto focusedWorldPosition = focusedEntity->getCuboidWorldSpace().topLeft;
        position.x = focusedWorldPosition.x;
        position.y = focusedWorldPosition.y;
        position.z = focusedWorldPosition.z;
    }
}

void Camera::focusOn(Entity* e) {
    focusedEntity = e;
}