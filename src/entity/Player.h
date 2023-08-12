#pragma once
#include <iostream>
#include "Entity.h"
#include <optional>

class MovementComponent;
class CollisionDetectorComponent;
class ControllableComponent;
class TextureComponent;
class ColorComponent;
class Floor;
class Player : public Entity {
public:
    Player() = default;
    Player(float elevationSpaceX, float elevationSpaceY, float elevationSpaceZ, float width, float height, float length, Elevation* elevation) :
        Entity(elevationSpaceX, elevationSpaceY, elevationSpaceZ, width, height, length, elevation) {}
    Player(float elevationSpaceX, float elevationSpaceY, Elevation* elevation) : Entity(elevationSpaceX, elevationSpaceY, elevation) {}
    Player(Elevation* elevation) : Entity(elevation) {}

    MovementComponent* movementComponent{nullptr};
    CollisionDetectorComponent* collisionDetectorComponent{nullptr};
    ControllableComponent* controllableComponent{nullptr};
    TextureComponent* textureComponent{nullptr};
    ColorComponent* colorComponent{nullptr};
};