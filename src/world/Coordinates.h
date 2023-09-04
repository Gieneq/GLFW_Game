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

    inline bool checkIntersection(const WorldCuboid& other) const {
        return cuboid.checkIntersection(other.cuboid);
    }

    static WorldCuboid transformCuboid(ElevationDepth* elevationDepth, const Cuboid6F& cuboid);

    static std::vector<WorldCuboid> transformCuboids(ElevationDepth* elevationDepth, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

    static WorldCuboid transformLocalCuboid(const ElevationCuboid& parent, const Cuboid6F& child);

    static std::vector<WorldCuboid> transformLocalCuboids(const ElevationCuboid& parent, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

    template <Side side>
    static WorldCuboid& getMaxSide(const std::vector<WorldCuboid>::const_iterator& worldCuboidsBegin, 
        const std::vector<WorldCuboid>::const_iterator& worldCuboidsEnd) {
        float maxSide = std::numeric_limits<float>::min();
        auto maxSideIt = worldCuboidsBegin;
        for (auto wCuboidsIt = worldCuboidsBegin; wCuboidsIt != worldCuboidsEnd; ++wCuboidsIt) {
            const auto& cuboid = (*wCuboidsIt).value();
            const auto side = cuboid.getSide<side>();
            if (side > maxSide) {
                maxSide = side;
                maxSideIt = wCuboidsIt;
            }
        }
        return *maxSideIt;
    }
    
    template <Side side>
    static WorldCuboid& getMinSide(const std::vector<WorldCuboid>::const_iterator& worldCuboidsBegin, 
        const std::vector<WorldCuboid>::const_iterator& worldCuboidsEnd) {
        float minSide = std::numeric_limits<float>::max();
        auto minSideIt = worldCuboidsBegin;
        for (auto wCuboidsIt = worldCuboidsBegin; wCuboidsIt != worldCuboidsEnd; ++wCuboidsIt) {
            const auto& cuboid = (*wCuboidsIt).value();
            const auto side = cuboid.getSide<side>();
            if (side < minSide) {
                minSide = side;
                minSideIt = wCuboidsIt;
            }
        }
        return *minSideIt;
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

    inline bool checkIntersection(const ElevationCuboid& other) const {
        return cuboid.checkIntersection(other.cuboid);
    }

    static ElevationCuboid transformCuboid(ElevationDepth* elevationDepth, const Cuboid6F& cuboid);

    static std::vector<ElevationCuboid> transformCuboids(ElevationDepth* elevationDepth, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

    static ElevationCuboid transformLocalCuboid(const ElevationCuboid& parent, const Cuboid6F& child);

    static std::vector<ElevationCuboid> transformLocalCuboids(const ElevationCuboid& parent, 
        const std::vector<Cuboid6F>::const_iterator& begin, const std::vector<Cuboid6F>::const_iterator& end);

    template <Side side>
    static const ElevationCuboid& getMaxSide(const std::vector<ElevationCuboid>::const_iterator& elevationCuboidsBegin, 
        const std::vector<ElevationCuboid>::const_iterator& elevationCuboidsEnd) {
        float maxSide = std::numeric_limits<float>::min();
        auto maxSideIt = elevationCuboidsBegin;
        for (auto eCuboidsIt = elevationCuboidsBegin; eCuboidsIt != elevationCuboidsEnd; ++eCuboidsIt) {
            const auto& cuboid = (*eCuboidsIt).value();
            const auto sideValue = cuboid.getSide<side>();
            if (sideValue > maxSide) {
                maxSide = sideValue;
                maxSideIt = eCuboidsIt;
            }
        }
        return *maxSideIt;
    }
    
    template <Side side>
    static const ElevationCuboid& getMinSide(const std::vector<ElevationCuboid>::const_iterator& elevationCuboidsBegin, 
        const std::vector<ElevationCuboid>::const_iterator& elevationCuboidsEnd) {
        float minSide = std::numeric_limits<float>::max();
        auto minSideIt = elevationCuboidsBegin;
        for (auto eCuboidsIt = elevationCuboidsBegin; eCuboidsIt != elevationCuboidsEnd; ++eCuboidsIt) {
            const auto& cuboid = (*eCuboidsIt).value();
            const auto sideValue = cuboid.getSide<side>();
            if (sideValue < minSide) {
                minSide = sideValue;
                minSideIt = eCuboidsIt;
            }
        }
        return *minSideIt;
    }

private:
    ElevationDepth* elevationDepth;
    Cuboid6F cuboid;
};

