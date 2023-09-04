#pragma once

#include <array>
#include <vector>
#include <optional>
#include <iostream>
#include "Maths.h"
#include "Coordinates.h"
#include "CustomContainers.h"

class Entity;


class FloorSegment3X3 : public Segment3X3<std::optional<Entity*>> {
public:
    Segment3X3Mask getMask() const;
};
