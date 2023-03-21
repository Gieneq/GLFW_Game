#include "Entity.h"


void Entity::addComponent(Component* component) {
    components.push_back(component);
}