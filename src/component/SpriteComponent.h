#pragma once
#include <iostream>
#include "Component.h"
#include "Maths.h"
#include "ImageData.h"

class ColorComponent : public Component {
public:
    virtual ~ColorComponent() = default;
    ColorComponent(Entity* e) : Component(e) {}
    float r{0.56f};
    float g{1.0f};
    float b{1.0f};
    Rect2F rect{0.0F,0.0F,1.0F,1.0F};
};

class TextureComponent : public Component {
public:
    virtual ~TextureComponent() = default;

    TextureComponent(Entity* e, const ImageId& id) : Component(e), image_id{id} {}
    ImageId image_id;
    Rect2F rect{0.0F,0.0F,1.0F,1.0F};
};
