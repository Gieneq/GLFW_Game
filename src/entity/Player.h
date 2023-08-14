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
    Player() = default;
    Player(Elevation* elevation) : Entity(elevation, EntityType::DYNAMIC) {}

    MovementComponent* movementComponent{nullptr};
    CollisionDetectorComponent* collisionDetectorComponent{nullptr};
    ControllableComponent* controllableComponent{nullptr};
    TextureComponent* textureComponent{nullptr};
    ColorComponent* colorComponent{nullptr};
};