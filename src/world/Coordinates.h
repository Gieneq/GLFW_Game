#pragma once
#include "Maths.h"
#include <vector>
#include <limits>

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

    static WorldCuboid transformCuboid(ElevationDepth* elevationDepth, const Cuboid6F& cuboid);

    static std::vector<WorldCuboid> transformCuboids(ElevationDepth* elevationDepth, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

    static WorldCuboid transformLocalCuboid(const ElevationCuboid& parent, const Cuboid6F& child);

    static std::vector<WorldCuboid> transformLocalCuboids(const ElevationCuboid& parent, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

    template <Side side>
    static float getMaxSide(const std::vector<WorldCuboid>::const_iterator& worldCuboidsBegin, 
        const std::vector<WorldCuboid>::const_iterator& worldCuboidsEnd) {
        float maxSide = std::numeric_limits<float>::min();
        for (auto wCuboidsIt = worldCuboidsBegin; wCuboidsIt != worldCuboidsEnd; ++wCuboidsIt) {
            const auto& cuboid = (*wCuboidsIt).cuboid.value();
            const auto side = cuboid.getSide<side>();
            if (sideLength > maxSide) {
                maxSide = sideLength;
            }
        }
        return maxSide;
    }
    
    template <Side side>
    static float getMinSide(const std::vector<WorldCuboid>::const_iterator& worldCuboidsBegin, 
        const std::vector<WorldCuboid>::const_iterator& worldCuboidsEnd) {
        float minSide = std::numeric_limits<float>::max();
        for (auto wCuboidsIt = worldCuboidsBegin; wCuboidsIt != worldCuboidsEnd; ++wCuboidsIt) {
            const auto& cuboid = (*wCuboidsIt).cuboid.value();
            const auto side = cuboid.getSide<side>();
            if (sideLength < minSide) {
                minSide = sideLength;
            }
        }
        return minSide;
    }

private:
    WorldCuboid(const Cuboid6F& cuboid) : cuboid{cuboid} {}
    Cuboid6F cuboid;
    friend class ElevationCuboid;
};

class ElevationCuboid {
public:
    ElevationCuboid(ElevationDepth* ed, const Cuboid6F& cuboid) : elevationDepth{ed}, cuboid{cuboid} {}
    ElevationCuboid(ElevationDepth* ed, float x, float y, float z, float w, float h, float d) : 
        elevationDepth{ed}, cuboid{x, y, z, w, h, d} {}
    
    WorldCuboid toWorldSpace() const;

    inline Cuboid6F& value() { return cuboid; }

    inline const Cuboid6F& value() const { return cuboid; }

    inline void setElevationDepth(ElevationDepth* ed) { elevationDepth = ed; }

    static ElevationCuboid transformCuboid(ElevationDepth* elevationDepth, const Cuboid6F& cuboid);

    static std::vector<ElevationCuboid> transformCuboids(ElevationDepth* elevationDepth, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

    static ElevationCuboid transformLocalCuboid(const ElevationCuboid& parent, const Cuboid6F& child);

    static std::vector<ElevationCuboid> transformLocalCuboids(const ElevationCuboid& parent, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

private:
    ElevationDepth* elevationDepth;
    Cuboid6F cuboid;
};

