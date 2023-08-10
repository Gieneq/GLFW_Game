#pragma once
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include "Camera.h"
#include "Loader.h"

class Entity;
class ColorComponent;
class TextureComponent;

struct EntityRenderData {
    const Entity* entity;
    float sortValue;

    bool operator<(const EntityRenderData& other) const {
        return sortValue < other.sortValue;
    }

    bool operator==(const EntityRenderData& other) const {
        return sortValue < other.sortValue;
    }
};

class RenderSystem : public SystemBase {
public:
    int system_id;
    void init();
    void prepare();
    void processEntity(const Entity* entity);
    void render(bool sorted = false);
    void setViewportDimensions(int width, int height);
    void attachCamera(Camera *cam);

    void renderTexturedBox(const TextureData& textureData, const Rect2F& worldRect, int tilesetIndex);
    void renderFilledBox(Rect2F worldRect, float r, float g, float b);
    void renderTranslucentFilledBox(Rect2F worldRect, float r, float g, float b, float fillingAlpha);
    
    int getLastEntitesCount() const {
        return lastEntitesCount;
    }

    Rect2F renderBoxWorldSpace;
    
private:
    void renderEntity(const Entity* entity);

    Camera *camera;
    int viewport_width{Settings::Window::WIDTH};
    int viewport_height{Settings::Window::HEIGHT};
    float aspect_ratio{Settings::Window::ASPECT_RATIO};

    int lastEntitesCount{0};
    std::vector<EntityRenderData> enititesBatch;
};

