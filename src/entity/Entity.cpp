#include "Entity.h"

int Entity::next_id = 0;


void Entity::addComponent(Component* component) {
    components.push_back(component);
}