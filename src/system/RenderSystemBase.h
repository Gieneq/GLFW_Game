#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include "Camera.h"
#include "Loader.h"

class ColorComponent;
class TextureComponent;

class RenderSystemBase : public SystemBase {
public:
    RenderSystemBase() : SystemBase() {}
    virtual ~RenderSystemBase() = default;
    int system_id;

    void init();
    void setViewportDimensions(int width, int height);
    void attachCamera(Camera *cam);

    Rect2F getProjectionRect(const Rect2F& worldRect) const;
    // Rect2F getProjectionRect(const Rect3F& worldRect) const;
    bool isProjectedRectVisible(const Rect2F& projectionRect) const;

    void renderTexturedBox(const Rect2F& worldRect, TextureData* textureData, int tilesetIndex);
    void renderFilledBox(const Rect2F& worldRect, float r, float g, float b);
    void renderTranslucentFilledBox(const Rect2F& worldRect, float r, float g, float b, float fillingAlpha);

    Rect2F getRenderBoxWorldSpace() const {
        return renderBoxWorldSpace;
    }
    bool debugView{false};

protected:
    virtual void loopStart();
    virtual void loopEnd();

    Camera *camera;
    Rect2F renderBoxWorldSpace;
    int viewport_width{Settings::Window::WIDTH};
    int viewport_height{Settings::Window::HEIGHT};
    float aspect_ratio{Settings::Window::ASPECT_RATIO};
};

