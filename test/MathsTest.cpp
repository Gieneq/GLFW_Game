#include <gtest/gtest.h>
#include <Maths.h>
#include <vector>

/* Rect4F */
TEST(Rect4FTest, Creation) {
    Rect4F rect;
    EXPECT_EQ(rect.x(), 0);
    EXPECT_EQ(rect.y(), 0);
    EXPECT_EQ(rect.w(), 0);
    EXPECT_EQ(rect.h(), 0);

    Rect4F rect2{1, 2, 3, 4};
    EXPECT_EQ(rect2.x(), 1);
    EXPECT_EQ(rect2.y(), 2);
    EXPECT_EQ(rect2.w(), 3);
    EXPECT_EQ(rect2.h(), 4);

    Rect4F rect3{{11, 22}, {33, 44}};
    EXPECT_EQ(rect3.x(), 11);
    EXPECT_EQ(rect3.y(), 22);
    EXPECT_EQ(rect3.w(), 33);
    EXPECT_EQ(rect3.h(), 44);
}

TEST(Rect4FTest, SidesAccess) {
    const float x = 1;
    const float y = 2;
    const float w = 4;
    const float h = 5;
    Rect4F rect{x, y, w, h};
    EXPECT_EQ(rect.left(), x);
    EXPECT_EQ(rect.right(), x + w);
    EXPECT_EQ(rect.top(), y);
    EXPECT_EQ(rect.bottom(), y + h);
}

TEST(Rect4FTest, PointInclusion) {
    const float x = 1;
    const float y = 2;
    const float w = 4;
    const float h = 5;
    const Rect4F rect{x, y, w, h};

    Vect2F p1Inside{x, y};
    Vect2F p2Inside{x + 0.1F, y};
    Vect2F p3Inside{x, y + 0.1F};
    Vect2F p4Inside{x + 0.2F, y + 0.1F};
    Vect2F p5Inside{x + w - 0.0001F, y + h - 0.0001F};
    EXPECT_TRUE(rect.hasPointInside(p1Inside));
    EXPECT_TRUE(rect.hasPointInside(p2Inside));
    EXPECT_TRUE(rect.hasPointInside(p3Inside));
    EXPECT_TRUE(rect.hasPointInside(p4Inside));
    EXPECT_TRUE(rect.hasPointInside(p5Inside));

    Vect2F p1Outside{x+w, y};
    Vect2F p2Outside{x, y+h};
    Vect2F p3Outside{x+w, y+h};
    EXPECT_FALSE(rect.hasPointInside(p1Outside));
    EXPECT_FALSE(rect.hasPointInside(p2Outside));
    EXPECT_FALSE(rect.hasPointInside(p3Outside));
}

TEST(Rect4FTest, CheckIntersection) {
    std::vector<std::pair<Rect4F, Rect4F>> notIntersetingRects = {
        {{0, 0, 1, 1}, {1, 0, 1, 1}},
        {{0, 0, 1, 1}, {0, 1, 1, 1}},
        {{0, 0, 1, 1}, {1, 1, 1, 1}},
        {{0, 0, 1, 1}, {-1, 0, 1, 1}},
        {{0, 0, 1, 1}, {0, -1, 1, 1}},
        {{0, 0, 1, 1}, {-1, -1, 1, 1}},
        {{0, 0, 1, 1}, {-1, 1, 1, 1}},
        {{0, 0, 1, 1}, {1, -1, 1, 1}},
    };

    for(const auto& pair : notIntersetingRects) {
        EXPECT_FALSE(pair.first.checkIntersection(pair.second)) << "Rects: " << pair.first << " and " << pair.second << " should not intersect";
        EXPECT_FALSE(pair.second.checkIntersection(pair.first));
        EXPECT_FALSE(Rect4F::checkIntersection(pair.first, pair.second));
        EXPECT_FALSE(Rect4F::checkIntersection(pair.second, pair.first));
    }

    std::vector<std::pair<Rect4F, Rect4F>> intersetingRects = {
        {{0, 0, 1, 1}, {0, 0, 1, 1}},
        {{0, 0, 1, 1}, {0.999F, 0, 1, 1}},
        {{0, 0, 1, 1}, {0, 0.999F, 1, 1}},
        {{0, 0, 1, 1}, {-0.999F, 0, 1, 1}},
        {{0, 0, 1, 1}, {0, -0.999F, 1, 1}},
        {{0, 0, 1, 1}, {0, 0, 0.5F, 0.5F}},
        {{0, 0, 1, 1}, {0.5F, 0.5F, 1, 1}},
        {{0, 0, 1, 1}, {0.5F, 0.5F, 0.5F, 0.5F}},
        {{0, 0, 1, 1}, {0.5F, 0, 0.5F, 1}},
        {{0, 0, 1, 1}, {0, 0.5F, 1, 0.5F}},
        {{0, 0, 1, 1}, {0, 0.5F, 0.5F, 0.5F}},
        {{0, 0, 1, 1}, {0.5F, 0, 0.5F, 0.5F}},
    };

        for(const auto& pair : intersetingRects) {
        EXPECT_TRUE(pair.first.checkIntersection(pair.second)) << "Rects: " << pair.first << " and " << pair.second << " should intersect";
        EXPECT_TRUE(pair.second.checkIntersection(pair.first));
        EXPECT_TRUE(Rect4F::checkIntersection(pair.first, pair.second));
        EXPECT_TRUE(Rect4F::checkIntersection(pair.second, pair.first));
    }

}

/* Cuboid6F */
TEST(Cuboid6FTest, Creation) {
    Cuboid6F cuboid;
    EXPECT_EQ(cuboid.x(), 0);
    EXPECT_EQ(cuboid.y(), 0);
    EXPECT_EQ(cuboid.z(), 0);
    EXPECT_EQ(cuboid.w(), 0);
    EXPECT_EQ(cuboid.h(), 0);
    EXPECT_EQ(cuboid.d(), 0);

    Cuboid6F cuboid2{1, 2, 3, 4, 5, 6};
    EXPECT_EQ(cuboid2.x(), 1);
    EXPECT_EQ(cuboid2.y(), 2);
    EXPECT_EQ(cuboid2.z(), 3);
    EXPECT_EQ(cuboid2.w(), 4);
    EXPECT_EQ(cuboid2.h(), 5);
    EXPECT_EQ(cuboid2.d(), 6);
}

TEST(Cuboid6FTest, SidesAccess) {
    const float x = 1;
    const float y = 2;
    const float z = 0;
    const float w = 4;
    const float h = 5;
    const float d = 1;
    Cuboid6F cuboid{x, y, z, w, h, d};
    EXPECT_EQ(cuboid.left(), x);
    EXPECT_EQ(cuboid.right(), x + w);
    EXPECT_EQ(cuboid.top(), y);
    EXPECT_EQ(cuboid.bottom(), y + h);
    EXPECT_EQ(cuboid.back(), z);
    EXPECT_EQ(cuboid.front(), z + d);
}

TEST(Cuboid6FTest, FlattenToRect) {
    const float x = 1;
    const float y = 2;
    const float z = 0;
    const float w = 4;
    const float h = 5;
    const float d = 1;
    Cuboid6F cuboid{x, y, z, w, h, d};
    const auto rect = cuboid.getFlatten();

    EXPECT_EQ(rect.left(), x);
    EXPECT_EQ(rect.right(), x + w);
    EXPECT_EQ(rect.top(), y);
    EXPECT_EQ(rect.bottom(), y + h);
}

TEST(Cuboid6FTest, CheckIntersection) {
    std::vector<std::pair<Cuboid6F, Cuboid6F>> notIntersectingCuboids {
        {{0,0,0,1,1,1}, {1,0,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,1,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,0,1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,0,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,-1,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,0,-1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,-1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,1,1,1,1,1}},
        {{0,0,0,1,1,1}, {1,-1,1,1,1,1}},
        {{0,0,0,1,1,1}, {1,1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {1,1,1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,-1,1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {1,-1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,1,1,1,1,1}},
        {{0,0,0,1,1,1}, {1,-1,1,1,1,1}},
        {{0,0,0,1,1,1}, {1,1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {1,1,1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,-1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,-1,1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {1,-1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {-1,1,1,1,1,1}},
        {{0,0,0,1,1,1}, {1,-1,1,1,1,1}},
        {{0,0,0,1,1,1}, {1,1,-1,1,1,1}},
        {{0,0,0,1,1,1}, {1,1,1,1,1,1}},
    };

    for (const auto& pair : notIntersectingCuboids) {
        EXPECT_FALSE(pair.first.checkIntersection(pair.second)) << "Cuboids: " << pair.first << " and " << pair.second << " should not intersect";
        EXPECT_FALSE(pair.second.checkIntersection(pair.first));
        EXPECT_FALSE(Cuboid6F::checkIntersection(pair.first, pair.second));
        EXPECT_FALSE(Cuboid6F::checkIntersection(pair.second, pair.first));
    }

    std::vector<std::pair<Cuboid6F, Cuboid6F>> intersectingCuboids {
        {{0,0,0,1,1,1}, {0.999F,0,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,0.999F,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,0,0.999F,1,1,1}},
        {{0,0,0,1,1,1}, {-0.999F,0,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,-0.999F,0,1,1,1}},
        {{0,0,0,1,1,1}, {0,0,-0.999F,1,1,1}},
        {{0,0,0,1,1,1}, {0,0,0,0.5F,0.5F,0.5F}},
        {{0,0,0,1,1,1}, {0.5F,0.5F,0.5F,1,1,1}},
        {{0,0,0,1,1,1}, {0.5F,0.5F,0.5F,1,1,1}},
        {{0,0,0,1,1,1}, {0.5F,0,0,0.5F,1,1}},
        {{0,0,0,1,1,1}, {0,0.5F,0,1,0.5F,1}},
        {{0,0,0,1,1,1}, {0,0,0.5F,1,1,0.5F}},
        {{0,0,0,1,1,1}, {0.5F,0,0,0.5F,1,1}},
        {{0,0,0,1,1,1}, {0,0.5F,0,1,0.5F,1}},
        {{0,0,0,1,1,1}, {0,0,0.5F,1,1,0.5F}},
    };

    for (const auto& pair : intersectingCuboids) {
        EXPECT_TRUE(pair.first.checkIntersection(pair.second)) << "Cuboids: " << pair.first << " and " << pair.second << " should intersect";
        EXPECT_TRUE(pair.second.checkIntersection(pair.first));
        EXPECT_TRUE(Cuboid6F::checkIntersection(pair.first, pair.second));
        EXPECT_TRUE(Cuboid6F::checkIntersection(pair.second, pair.first));
    }

}