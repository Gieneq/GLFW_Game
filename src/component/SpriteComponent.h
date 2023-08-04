#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include "TextureId.h"


// class TransparentComponent : public Component

class ColorComponent : public Component {
public:
    ColorComponent(Entity* e) : Component(e) {}
    virtual ~ColorComponent() = default;
    float r{0.56f};
    float g{1.0f};
    float b{1.0f};
    Rect2F worldRect{0.0F,0.0F,1.0F,1.0F};

    ColorComponent* clone(Entity* new_parent) override {
        return new ColorComponent(new_parent);
    }
};

class TextureComponent : public Component {
public:
    TextureComponent(Entity* e, TextureID id) : Component(e), textureID{id} {}
    TextureComponent(Entity* e, TextureID id, int tilesetIdx) : Component(e), textureID{id}, tilesetIndex{tilesetIdx} {}
    virtual ~TextureComponent() = default;

    TextureID textureID{};
    int tilesetIndex{0};
    Rect2F worldRect{0.0F,0.0F,1.0F,1.0F};

    TextureComponent* clone(Entity* new_parent) override {
        return new TextureComponent(new_parent, textureID);
    }
};
