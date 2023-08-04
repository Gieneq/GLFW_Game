#include "Camera.h"
#include "Maths.h"
#include "Entity.h"
#include "LocationComponent.h"


void Camera::update(float dt) {
    if(focusedLocation) {
        position.x = focusedLocation->worldRect.top_left.x;
        position.y = focusedLocation->worldRect.top_left.y;
    }
}

void Camera::focusOn(LocationComponent *loc) {
    focusedLocation = loc;
}