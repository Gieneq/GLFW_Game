#include "Coordinates.h"
#include "World.h"

WorldCuboid ElevationCuboid::toWorldSpace() const {
    auto tmpCuboid = cuboid;
    tmpCuboid.z() += elevationDepth->z();
    return WorldCuboid{tmpCuboid};
}