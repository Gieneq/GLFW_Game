#include "EntityContainers.h"
#include "CollisionComponents.h"
#include "World.h"

Segment3X3Mask FloorSegment3X3::getMask() const {
    Segment3X3Mask mask;
    for (int i = 0; i < 9; i++) {
        mask.set(i, get(i).has_value());
    }
    return mask;
}
