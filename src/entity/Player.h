#pragma once
#include <iostream>
#include "Entity.h"
#include <optional>

class LocationComponent;
class MovementComponent;
class CollisionDetectorComponent;
class ControllableComponent;
class TextureComponent;
class ColorComponent;
class Floor;
class Player : public Entity {
public:
    Player() = default;

    bool isOnFloor() const;
    std::optional<Floor*> getFloor() const;

    LocationComponent* locationComponent{nullptr};
    MovementComponent* movementComponent{nullptr};
    CollisionDetectorComponent* collisionDetectorComponent{nullptr};
    ControllableComponent* controllableComponent{nullptr};
    TextureComponent* textureComponent{nullptr};
    ColorComponent* colorComponent{nullptr};
};