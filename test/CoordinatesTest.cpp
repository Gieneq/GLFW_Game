#include "CoordinatesTest.h"
#include <gtest/gtest.h>

#include <Maths.h>
#include <Coordinates.h>
// #include <vector>

/* Elevation Cuboid */
TEST(ElevationCuboidTest, Creation) {
    MockElevation mockElevation;
    const ElevationCuboid elevationCuboid(&mockElevation, -1, 0, 0, 2, 1, 1);
    EXPECT_EQ(elevationCuboid.value().left(), -1);
    EXPECT_EQ(elevationCuboid.value().right(), 1);
    EXPECT_EQ(elevationCuboid.value().w(), 2);
}


/* World Cuboid */
TEST(WorldCuboidTest, Creation) {
    MockElevation mockElevation;
    const int elevationIndex{2};

    const ElevationCuboid elevationCuboid(&mockElevation, 0, 0, 0, 1, 1, 1);
    EXPECT_EQ(elevationCuboid.value().z(), 0);

    EXPECT_CALL(mockElevation, z()).WillOnce(::testing::Return(static_cast<float>(elevationIndex)));

    const WorldCuboid worldCuboid = elevationCuboid.toWorldSpace();
    EXPECT_EQ(worldCuboid.value().z(), elevationIndex);
    
    EXPECT_CALL(mockElevation, getIndex()).WillOnce(::testing::Return(elevationIndex));
    EXPECT_EQ(mockElevation.getIndex(), elevationIndex);
}

