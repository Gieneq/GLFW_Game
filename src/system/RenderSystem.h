#pragma once
#include "RenderSystemBase.h"
#include <iostream>
#include <vector>
#include "SystemBase.h"
#include "Settings.h"
#include "Camera.h"
#include "Loader.h"



class Entity;
struct EntityBatchData {
    EntityBatchData(Entity* entity) : entity{entity} {
        sortValue = entity->getCuboid().value().bottom() + entity->getCuboid().value().z();
    }
    
    Entity* entity;
    float sortValue;

    bool operator<(const EntityBatchData& other) const {
        return sortValue < other.sortValue;
    }

    bool operator==(const EntityBatchData& other) const {
        return sortValue == other.sortValue;
    }
};

class Elevation;
class CollisionComponent;
class RenderSystem : public RenderSystemBase {
public:
    RenderSystem() : RenderSystemBase() {}
    virtual ~RenderSystem() = default;

    void loopStart() override;
    void loopEnd() override;

    void batchStart();
    void temporaryBatchAppendEntity(Entity* entity);
    void batchAppendElevation(Elevation* elevation);
    void batchEnd(bool sorted = false);
    void renderBatch();

    void renderCollisionBoxes(int elevationIndex,
        std::vector<CollisionComponent*>::const_iterator collisionCmpsBegin,
        std::vector<CollisionComponent*>::const_iterator collisionCmpsEnd);

    void fillScreen(float r, float g, float b, float a);

    
    inline int getLastBatchEntitesCount() const {
        return lastBatchEntitesCount;
    }

    inline int getLastLoopEntitesCount() const {
        return lastBatchEntitesCount;
    }

    int getClipElevationIndex() {
        return clipElevationIndex;
    }
    
    void setClipElevationIndex(int index) {
        clipElevationIndex = index;
        if(clipElevationIndex < 0) {
            clipElevationIndex = 0;
        }
    }

private:
    void renderEntityData(const EntityBatchData& entityData);
    int lastBatchEntitesCount{0};

    int recentLoopEntitesCount{0};
    int lastLoopEntitesCount{0};

    bool batchRenderElevationsProceed{true};
    int clipElevationIndex{6};

    std::vector<EntityBatchData> temporaryBatch;
    std::vector<EntityBatchData> enititesBatch;
};


