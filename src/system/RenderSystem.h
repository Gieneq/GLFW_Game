#pragma once
#include "RenderSystemBase.h"
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include "Camera.h"
#include "Loader.h"


class Entity;
class Elevation;
class CollisionComponent;

struct EntityBatchData {
    const Entity* entity;
    float sortValue;

    bool operator<(const EntityRenderData& other) const {
        return sortValue < other.sortValue;
    }

    bool operator==(const EntityRenderData& other) const {
        return sortValue < other.sortValue;
    }
};

class RenderSystem : public RenderSystemBase {
public:
    RenderSystem() : RenderSystemBase() {}

    
    void loopStart() override;
    void loopEnd() override;
    void batchStart();
    void batchAppendEntity(Entity* e);
    void batchEnd(bool sorted = false);
    void renderBatch();

    
    inline int getLastBatchEntitesCount() const {
        return lastBatchEntitesCount;
    }

    inline int getLastLoopEntitesCount() const {
        return lastBatchEntitesCount;
    }

private:
    int lastBatchEntitesCount{0};

    int recentLoopEntitesCount{0};
    int lastLoopEntitesCount{0};

    std::vector<EntityBatchData> enititesBatch;
};
























// class Entity;
// class Elevation;
// class ColorComponent;
// class TextureComponent;
// class CollisionComponent;

// class RenderSystem : public RenderSystemBase {
// public:
//     int system_id;
//     void init();
//     void loopStart();
//     void loopEnd();
//     void prepareBatch();
//     void prepareBatch();
//     void processElevation(const Elevation* elevation);
//     void processEntity(const Entity* entity);
//     void render(bool sorted = false);
//     void setViewportDimensions(int width, int height);
//     void attachCamera(Camera *cam);

//     void renderTexturedBox(const TextureData& textureData, const Rect2F& worldRect, int tilesetIndex);
//     void renderFilledBox(Rect2F worldRect, float r, float g, float b);
//     void renderTranslucentFilledBox(Rect2F worldRect, float r, float g, float b, float fillingAlpha);
    
//     void renderCollisionBoxes(const std::vector<CollisionComponent*>& collisionComponents);

//     int getLastEntitesCount() const {
//         return lastEntitesCount;
//     }

//     Rect2F renderBoxWorldSpace;
//     bool rendeDebugView{false};
    
// private:
//     void renderElevation(const Elevation* elevation);
//     void renderEntity(const Entity* entity);

//     Camera *camera;
//     int viewport_width{Settings::Window::WIDTH};
//     int viewport_height{Settings::Window::HEIGHT};
//     float aspect_ratio{Settings::Window::ASPECT_RATIO};

//     int lastEntitesCount{0};
//     std::vector<EntityRenderData> enititesBatch;
//     srd::vector<Elevation*> elevationsBatch;
// };

