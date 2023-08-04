#include "Camera.h"
#include "Maths.h"
#include "Entity.h"
#include "TransformComponent.h"


void Camera::update(float dt) {
    if(focused_component) {
        position.x = focused_component->target.x;
        position.y = focused_component->target.y;
    }
}

void Camera::focus_on(Entity *e) {
    if(e->hasComponent<TransformComponent>()) {
        this->focused_entity = e;
        this->focused_component = e->getComponent<TransformComponent>();
    }
}