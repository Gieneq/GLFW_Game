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

    Rect4F getProjectionRect(const Rect4F& worldRect) const;
    // Rect4F getProjectionRect(const Rect3F& worldRect) const;
    bool isProjectedRectVisible(const Rect4F& projectionRect) const;

    void renderTexturedRect4F(const Rect4F& worldRect, TextureData* textureData, int tilesetIndex);
    void renderFilledRect4F(const Rect4F& worldRect, float r, float g, float b);
    void renderTranslucentFilledRect4F(const Rect4F& worldRect, float r, float g, float b, float fillingAlpha);
    void renderFilledCuboid5F(const Cuboid5F& worldCuboid, float r, float g, float b);
    void renderFilledCuboid6F(const Cuboid6F& worldCuboid, float r, float g, float b);
    void renderTranslucentFilledCuboid5F(const Cuboid5F& worldCuboid, float r, float g, float b, float fillingAlpha);
    void renderTranslucentFilledCuboid6F(const Cuboid6F& worldCuboid, float r, float g, float b, float fillingAlpha);

    Rect4F getRenderBoxWorldSpace() const {
        return renderBoxWorldSpace;
    }
    bool debugView{false};

protected:
    virtual void loopStart();
    virtual void loopEnd();

    Camera *camera;
    Rect4F renderBoxWorldSpace;
    int viewport_width{Settings::Window::WIDTH};
    int viewport_height{Settings::Window::HEIGHT};
    float aspect_ratio{Settings::Window::ASPECT_RATIO};
};

