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
public:
    Player() : Entity(nullptr, EntityType::DYNAMIC) {};
    Player(Elevation* elevation) : Entity(elevation, EntityType::DYNAMIC) {}

    MovementComponent* addMovementComponent(float speed);
    std::optional<CollisionDetectorComponent*> addCollisionDetectorComponent(const Rect2F& boundingRect);  
    std::optional<ControllableComponent*> addControllableComponent();

    MovementComponent* movementComponent{nullptr};
    CollisionDetectorComponent* collisionDetectorComponent{nullptr};
    ControllableComponent* controllableComponent{nullptr};
};