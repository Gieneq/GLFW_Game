#pragma once
#include "Maths.h"

/**
 * Semantic types for easier distinguishing between different coordinate systems.
*/

class ElevationDepth;
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
    ElevationCuboid(ElevationDepth* ed, const Cuboid6F& cuboid) : elevationDepth{ed}, cuboid{cuboid} {}
    ElevationCuboid(ElevationDepth* ed, float x, float y, float z, float w, float h, float d) : elevationDepth{ed}, cuboid{x, y, z, w, h, d} {}
    
    WorldCuboid toWorldSpace() const;

    inline Cuboid6F& value() {
        return cuboid;
    }

    inline const Cuboid6F& value() const {
        return cuboid;
    }

private:
    ElevationDepth* elevationDepth;
    Cuboid6F cuboid;
};

