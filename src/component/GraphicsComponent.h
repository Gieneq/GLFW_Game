#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include "TextureId.h"

class LocationComponent;
class ColorComponent : public Component {
public:
    ColorComponent(Entity* e, LocationComponent* loc) : Component(e), parentLocation(loc) {}
    virtual ~ColorComponent() = default;
    float r{0.56f};
    float g{1.0f};
    float b{1.0f};
    LocationComponent* parentLocation{nullptr};
    Rect2F relativeBoxRect{0.0F,0.0F,1.0F,1.0F};
    Rect2F getWorldRect();

    ColorComponent* clone(Entity* new_parent) override {
        //todo fix
        return new ColorComponent(new_parent, parentLocation);
    }
};

class TextureComponent : public Component {
public:
    TextureComponent(Entity* e, LocationComponent* loc, TextureID id) : Component(e), parentLocation(loc), textureID{id} {}
    TextureComponent(Entity* e, LocationComponent* loc, TextureID id, int tilesetIdx) : Component(e), parentLocation(loc), textureID{id}, tilesetIndex{tilesetIdx} {}
    virtual ~TextureComponent() = default;

    TextureID textureID{};
    int tilesetIndex{0};
    LocationComponent* parentLocation{nullptr};
    Rect2F relativeBoxRect{0.0F,0.0F,1.0F,1.0F};
    Rect2F getWorldRect();

    TextureComponent* clone(Entity* new_parent) override {
        //todo fix
        return new TextureComponent(new_parent, parentLocation, textureID);
    }
};
