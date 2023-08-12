#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include "TextureId.h"

class ColorComponent : public Component {
public:
    ColorComponent(Entity* e, float relX, float relY, float boxWidth, float boxHeight) : Component(e), size{boxWidth, boxHeight} {}
    virtual ~ColorComponent() = default;
    Vect2F getPositionElevationSpace() const;
    Rect2F getRectElevationSpace() const;
    Vect3F getPositionWorldSpace() const;
    Size2F getSize() const { return size; }
    float r{0.56f};
    float g{1.0f};
    float b{1.0f};
    float a{1.0f};

private:
    Vect2F relativeBoxTranslation{0.0F, 0.0F};
    Size2F size{0.0F, 0.0F};
};

class TextureComponent : public Component {
public:
    TextureComponent(Entity* e, float relX, float relY, float boxWidth, float boxHeight, TextureID id) : Component(e), textureID{id} {}
    TextureComponent(Entity* e, float relX, float relY, float boxWidth, float boxHeight, TextureID id, int tilesetIdx) : Component(e), textureID{id}, tilesetIndex{tilesetIdx} {}
    virtual ~TextureComponent() = default;

    Vect2F getPositionElevationSpace() const;
    Rect2F getRectElevationSpace() const;
    Vect3F getPositionWorldSpace() const;
    Size2F getSize() const { return size; }

    TextureID getTextureID() const { return textureID; }
    int getTilesetIndex() const { return tilesetIndex; }
    void setTilesetIndex(int idx) { tilesetIndex = idx; }

private:
    Vect2F relativeBoxTranslation{0.0F, 0.0F};
    Size2F size{0.0F, 0.0F};
    TextureID textureID{};
    int tilesetIndex{0};
};
