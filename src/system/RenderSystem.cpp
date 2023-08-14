#include "RenderSystem.h"
#include "GLCommon.h"
#include "Window.h"
#include "Loader.h"
#include "GraphicsComponent.h"
#include "CollisionComponents.h"
#include "Entity.h"
#include <algorithm>
#include "Entity.h"

void RenderSystem::loopStart() {
    RenderSystemBase::loopStart();
}

void RenderSystem::loopEnd() {
    RenderSystemBase::loopEnd();
    
    lastBatchEntitesCount = recentLoopEntitesCount;
    recentLoopEntitesCount = 0;
}


void RenderSystem::batchStart() {
    enititesBatch.clear();
    
    /* Cleare render box in world space from camera position */
    // todo move to camera
    renderBoxWorldSpace.size.w = 2.0F / (1.0F * camera->zoom.w * (1.0F / aspect_ratio));
    renderBoxWorldSpace.size.h = 2.0F / (1.0F * camera->zoom.h);
    renderBoxWorldSpace.topLeft.x = camera->position.x - renderBoxWorldSpace.size.w / 2.0F;
    renderBoxWorldSpace.topLeft.y = camera->position.y - renderBoxWorldSpace.size.h / 2.0F;
    //todo probably 3D
}

void RenderSystem::batchAppendEntity(Entity* e) {
    if(renderBoxWorldSpace.checkIntersection(e->getCuboidWorldSpace().getFlatten())) {
        enititesBatch.push_back(EntityBatchData {
            e, 
            e->getCuboidElevationSpace()->bottom()
        });
    }
}

void RenderSystem::batchAppendElevation(Elevation* elevation) {
    /* Floor flat tiles */
    for(auto entity: elevation->getFloorEntities()) {
        this->batchAppendEntity(entity);
    }

    /* Clutter flat tiles */
    for(auto entity: elevation->getClutterEntities()) {
        this->batchAppendEntity(entity);
    }
    
    /* Other big tiles */
    std::vector<EntityBatchData> tmpBatch;
    for(auto entity: elevation->getBiggerEntities()) {
        tmpBatch.push_back(EntityBatchData {
            entity, 
            entity->getCuboidElevationSpace()->bottom()
        });
    }
    std::sort(tmpBatch.begin(), tmpBatch.end());
    enititesBatch.insert(enititesBatch.end(), tmpBatch.begin(), tmpBatch.end());
}

void RenderSystem::batchEnd(bool sorted) {
    if (sorted) {
        // std::sort(enititesBatch.begin(), enititesBatch.end(), [](const Entity* a, const Entity* b) {
        //     return a->position.z < b->position.z; #Err
        // });
        std::sort(enititesBatch.begin(), enititesBatch.end());
    }
    recentLoopEntitesCount += static_cast<int>(enititesBatch.size());
}

void RenderSystem::renderBatch() {
    /* Render entities */
    for(auto entityData : enititesBatch) {
        renderEntityData(entityData);
    }
}


void RenderSystem::renderEntityData(const EntityBatchData& entityData) {
    /* Textured quad rendering */
    auto entity = entityData.entity;
    auto textureCmpOption = entity->getTextureComponent();
    if(textureCmpOption.has_value()) {
        auto textureCmp = textureCmpOption.value();

        /* Check if has valid texture */
        auto textureDataOption = Loader::getLoader().getTextureDataByID(textureCmp->getTextureID());
        if(!textureDataOption.has_value()) {
            /* Corrupted */
            renderFilledBox(textureCmp->getRectElevationSpace(), 1.0F, 0.0F, 0.0F);
        }
        else {
            renderTexturedBox(textureCmp->getRectElevationSpace(), textureDataOption.value(), textureCmp->getTilesetIndex());
        }
    }

    /* Colored quad rendering */
    auto colorCmpOption = entity->getColorComponent();
    if(colorCmpOption.has_value()) {
        auto colorCmp = colorCmpOption.value();
        //todo colorCmp->a
        renderFilledBox(colorCmp->getRectElevationSpace(), colorCmp->r, colorCmp->g, colorCmp->b);
    }
}


void RenderSystem::renderCollisionBoxes(const std::vector<CollisionComponent*>& collisionComponents) {
    for(auto collisionCmp : collisionComponents) {
        // renderFilledBox(collisionCmp->getRectElevationSpace(), 1.0F, 0.0F, 0.0F);
            // renderFilledBox(box, 1.0F, 0.0F, 0.0F);
        for (auto& box : collisionCmp->getElevationSpaceCollisionRects()) {
            renderTranslucentFilledBox(box, 1.0F, 0.0F, 0.0F, 0.5F);
        }
    }
}




