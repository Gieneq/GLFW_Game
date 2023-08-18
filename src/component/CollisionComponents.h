#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include <vector>


class CollisionComponent : public Component {
public:
    CollisionComponent(Entity* e) : Component(e) {}
    virtual ~CollisionComponent() = default;


    void appendCollidionRect(const Rect4F& rect) {
        collisionRects.push_back(rect);
    }

    std::vector<Rect4F> getElevationSpaceCollisionRects() const;
    std::vector<Rect6F> getWorldSpaceCollisionRects() const;

private:
    std::vector<Rect4F> collisionRects;
};


class MovementComponent;
class CollisionDetectorComponent : public Component {
public:
    CollisionDetectorComponent(Entity* e, MovementComponent* movmnt, const Rect4F& bnd) : Component(e), movementCmp{movmnt}, boundingRect{bnd} {}
    virtual ~CollisionDetectorComponent() = default;

    void onCollision(std::vector<Rect4F> collidingRectsElevationSpace);

    MovementComponent* movementCmp{nullptr};
    Rect4F getElevationSpaceBoundingRect() const;
    Rect6F getWorldSpaceBoundingRect() const;

private:
    Rect4F boundingRect{0, 0.15F, 1, 0.85F};
};

