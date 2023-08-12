#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include <vector>


class CollisionComponent : public Component {
public:
    CollisionComponent(Entity* e) : Component(e) {}
    virtual ~CollisionComponent() = default;


    void appendCollidionRect(const Rect2F& rect) {
        collisionRects.push_back(rect);
    }

    std::vector<Rect2F> getElevationSpaceCollisionRects() const;
    std::vector<Rect3F> getWorldSpaceCollisionRects() const;

private:
    std::vector<Rect2F> collisionRects;
};


class MovementComponent;
class CollisionDetectorComponent : public Component {
public:
    CollisionDetectorComponent(Entity* e, MovementComponent* movmnt, const Rect2F& bnd) : Component(e), movementCmp{movmnt}, boundingRect{bnd} {}
    virtual ~CollisionDetectorComponent() = default;

    void onCollision(std::vector<Rect2F> collidingRectsElevationSpace);

    MovementComponent* movementCmp{nullptr};
    Rect2F getElevationSpaceBoundingRect() const;

private:
    Rect2F boundingRect{0, 0.1F, 1, 0.9F};
};

