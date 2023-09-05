#pragma once

#include <array>
#include <vector>
#include <optional>
#include <iostream>
#include "Maths.h"
#include "Coordinates.h"
#include "CustomContainers.h"
#include "Entity.h"

template <EntityType type>
class EntitySegment3X3 : public Segment3X3<std::optional<Entity*>> {
public:
    EntitySegment3X3() : Segment3X3<std::optional<Entity*>>(), entitiesType{type} {}    

    Segment3X3Mask getMask() const {
        Segment3X3Mask mask;
        for (int i = 0; i < 9; i++) {
            mask.set(i, get(i).has_value());
        }
        return mask;
    }

private:
    EntityType entitiesType;
};
