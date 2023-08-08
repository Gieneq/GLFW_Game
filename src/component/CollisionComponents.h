#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include <vector>


class LocationComponent;
class CollisionComponent : public Component {
public:
    CollisionComponent(Entity* e, LocationComponent* loc) : Component(e), parentLocation{loc} {}
    virtual ~CollisionComponent() = default;

    LocationComponent* parentLocation{nullptr};
    std::vector<Rect2F> collisionRects;

    void appendCollidionRect(const Rect2F& rect) {
        collisionRects.push_back(rect);
    }

    std::vector<Rect2F> getWorldSpaceCollisionRects() const;

    CollisionComponent* clone(Entity* new_parent) override {
        return new CollisionComponent(new_parent, parentLocation);
    }
};

class MovementComponent;
class CollisionDetectorComponent : public Component {
public:
    CollisionDetectorComponent(Entity* e, MovementComponent* movmnt) : Component(e), movementCmp{movmnt} {}
    virtual ~CollisionDetectorComponent() = default;

    void onCollision(std::vector<Rect2F> collidingRects);

    MovementComponent* movementCmp{nullptr};
    Rect2F boundingRect{0, 0, 1, 0.12F};
    Rect2F getWorldSpaceBoundingRect() const;

    CollisionDetectorComponent* clone(Entity* new_parent) override {
        return new CollisionDetectorComponent(new_parent, movementCmp);
    }
};

