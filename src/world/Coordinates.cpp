#include "Coordinates.h"
#include "World.h"

/* World */
WorldCuboid WorldCuboid::transformCuboid(ElevationDepth* elevationDepth, const Cuboid6F& cuboid) {
    return ElevationCuboid::transformCuboid(elevationDepth, cuboid).toWorldSpace();
}

std::vector<WorldCuboid> WorldCuboid::transformCuboids(ElevationDepth* elevationDepth, 
    const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end) {
    std::vector<WorldCuboid> worldCuboids;
    for (auto it = begin; it != end; ++it) {
        worldCuboids.push_back(WorldCuboid::transformCuboid(elevationDepth, *it));
    }
    return worldCuboids;
}

/* Wolrd - in relation */

WorldCuboid WorldCuboid::transformLocalCuboid(const ElevationCuboid& parent, const Cuboid6F& child) {
    return ElevationCuboid::transformLocalCuboid(parent, child).toWorldSpace();
}

std::vector<WorldCuboid> WorldCuboid::transformLocalCuboids(const ElevationCuboid& parent, 
    const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end) {
    std::vector<WorldCuboid> worldCuboids;
    for (auto it = begin; it != end; ++it) {
        worldCuboids.push_back(WorldCuboid::transformLocalCuboid(parent, *it));
    }
    return worldCuboids;
}

/* Elevation */

WorldCuboid ElevationCuboid::toWorldSpace() const {
    auto tmpCuboid = cuboid;
    tmpCuboid.z() += elevationDepth->z();
    return WorldCuboid{tmpCuboid};
}

ElevationCuboid ElevationCuboid::transformCuboid(ElevationDepth* elevationDepth, const Cuboid6F& cuboid) {
    return ElevationCuboid{elevationDepth, cuboid};
}

std::vector<ElevationCuboid> ElevationCuboid::transformCuboids(ElevationDepth* elevationDepth, 
    const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end) {
    std::vector<ElevationCuboid> elevationCuboids;
    for (auto it = begin; it != end; ++it) {
        elevationCuboids.push_back(ElevationCuboid::transformCuboid(elevationDepth, *it));
    }
    return elevationCuboids;
}

/* Elevation - in relation */
ElevationCuboid ElevationCuboid::transformLocalCuboid(const ElevationCuboid& parent, const Cuboid6F& child) {
    // return ElevationCuboid{parent.elevationDepth, parent.value().getTranslated(child.topLeftBottom())};
    return ElevationCuboid{parent.elevationDepth, child.getTranslated(parent.value().topLeftBottom())};
}

std::vector<ElevationCuboid> ElevationCuboid::transformLocalCuboids(const ElevationCuboid& parent, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end) {
    std::vector<ElevationCuboid> elevationCuboids;
    for (auto it = begin; it != end; ++it) {
        elevationCuboids.push_back(ElevationCuboid::transformLocalCuboid(parent, *it));
    }
    return elevationCuboids;
}