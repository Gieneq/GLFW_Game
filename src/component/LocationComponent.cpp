#include "LocationComponent.h"
#include "World.h"
#include <optional>

float LocationComponent::getWorldZ() const {
    return containingFloor->elevation + relativeZ;
}

int LocationComponent::getRecentElevation() {
    return containingFloor->elevation;
}

bool LocationComponent::changeElevation(int newElevation) {
    if(newElevation == getRecentElevation()) {
        return true;
    }

    /* Get next containing floor, check if exist */
    auto world = containingFloor->getContainingWorld();
    auto nextFloorOption = world->getFloor(newElevation);
    if(!nextFloorOption.has_value()) {
        return false;
    }

    /* Get next containing floor */
    auto nextFloor = nextFloorOption.value();

    /* Remove from current floor */
    containingFloor->removeEntity(parent);

    /* Add to next floor */
    nextFloor->addDynamicEntity(parent);

    return true;
}