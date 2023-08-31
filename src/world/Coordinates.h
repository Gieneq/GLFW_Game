#pragma once
#include <iostream>
#include "Maths.h"

/**
 * Semantic types for easier distinguishing between different coordinate systems.
*/

class Elevation;
class ElevationCuboid;

class WorldCuboid {
public:
    inline Cuboid6F& value() {
        return cuboid;
    }

    inline const Cuboid6F& value() const {
        return cuboid;
    }

private:
    WorldCuboid(const Cuboid6F& cuboid) : cuboid{cuboid} {}
    Cuboid6F cuboid;
    friend class ElevationCuboid;
};

class ElevationCuboid {
public:
    ElevationCuboid(Elevation* elevation, const Cuboid6F& cuboid) : elevation{elevation}, cuboid{cuboid} {}
    ElevationCuboid(Elevation* elevation, float x, float y, float z, float w, float h, float d) : elevation{elevation}, cuboid{x, y, z, w, h, d} {}
    
    WorldCuboid toWorldSpace() const;

    inline Cuboid6F& value() {
        return cuboid;
    }

    inline const Cuboid6F& value() const {
        return cuboid;
    }

private:
    Elevation* elevation;
    Cuboid6F cuboid;
};

