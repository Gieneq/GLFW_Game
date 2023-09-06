#pragma once
#include <iostream>
#include "Entity.h"
#include <optional>

class MovementComponent;
class CollisionDetectorComponent;
class ControllableComponent;
class TextureComponent;
class ColorComponent;
class Elevation;

class Player : public Entity {
private:
    Player() : Entity(nullptr, Entity::Type::DYNAMIC) {};
    Player(Elevation* elevation) : Entity(elevation, Entity::Type::DYNAMIC) {}
    ~Player() = default;

public:
    MovementComponent* addMovementComponent(float speed) override;
    std::optional<CollisionDetectorComponent*> addCollisionDetectorComponent(const Cuboid6F& boundingCuboid) override ;  
    std::optional<ControllableComponent*> addControllableComponent() override ;


    /* Access quick pointers - they are set for sure */
    inline MovementComponent* getMovementComponent() const {
        return movementComponent;
    }

    inline CollisionDetectorComponent* getCollisionDetectorComponent() const {
        return collisionDetectorComponent;
    }

    inline ControllableComponent* getControllableComponent() const {
        return controllableComponent;
    }

protected:
    MovementComponent* movementComponent{nullptr};
    CollisionDetectorComponent* collisionDetectorComponent{nullptr};
    ControllableComponent* controllableComponent{nullptr};

    friend class World;
};