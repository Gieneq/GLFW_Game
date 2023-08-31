#include "EntitiesList.h"
#include "CollisionComponents.h"
#include "World.h"

void EntitiesList::remove(Entity* entity) {
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if (*it == entity) {
            entities.erase(it);
            return;
        }
    }
}

