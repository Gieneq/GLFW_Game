#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include "TextureId.h"

class ColorComponent : public Component {
public:
    ColorComponent(Entity* e, float relX, float relY, float boxWidth, float boxHeight) : 
        Component(e), rectRelative{relX, relY, boxWidth, boxHeight} {}
    virtual ~ColorComponent() = default;

    Rect4F* getRectRelative() {
        return &rectRelative;
    }

    Rect4F getRectElevationSpace() const;

    Rect6F getCubiodWorldSpace() const;

    float r{0.56f};
    float g{1.0f};
    float b{1.0f};
    float a{1.0f};

private:
    Rect4F rectRelative{0.0F, 0.0F, 1.0F, 1.0F};
};

class TextureComponent : public Component {
public:
    TextureComponent(Entity* e, float relX, float relY, float boxWidth, float boxHeight, TextureID id) : Component(e), rectRelative(relX, relY, boxWidth, boxHeight), textureID{id} {}
    TextureComponent(Entity* e, float relX, float relY, float boxWidth, float boxHeight, TextureID id, int tilesetIdx) : Component(e), textureID{id}, rectRelative(relX, relY, boxWidth, boxHeight), tilesetIndex{tilesetIdx} {}
    virtual ~TextureComponent() = default;

    Rect4F* getRectRelative() {
        return &rectRelative;
    }

    Rect4F getRectElevationSpace() const;

    Rect6F getCubiodWorldSpace() const;

    TextureID getTextureID() const { return textureID; }

    int getTilesetIndex() const { return tilesetIndex; }

    void setTilesetIndex(int idx) { tilesetIndex = idx; }

private:
    Rect4F rectRelative{0.0F, 0.0F, 1.0F, 1.0F};
    TextureID textureID{};
    int tilesetIndex{0};
};
