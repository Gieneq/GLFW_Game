#include "RenderSystem.h"
#include "GLCommon.h"
#include "Window.h"
#include "Loader.h"
#include "GraphicsComponent.h"
#include "CollisionComponents.h"
#include "Entity.h"
#include <algorithm>
#include "World.h"

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
    temporaryBatch.clear();
    batchRenderElevationsProceed = true;
    
    /* Cleare render box in world space from camera position */
    // todo move to camera
    renderBoxWorldSpace.size.w = 2.0F / (1.0F * camera->zoom.w * (1.0F / aspect_ratio));
    renderBoxWorldSpace.size.h = 2.0F / (1.0F * camera->zoom.h);
    renderBoxWorldSpace.topLeft.x = camera->position.x - renderBoxWorldSpace.size.w / 2.0F;
    renderBoxWorldSpace.topLeft.y = camera->position.y - renderBoxWorldSpace.size.h / 2.0F;
    //todo probably 3D
}

void RenderSystem::temporaryBatchAppendEntity(Entity* entity) {
    if(renderBoxWorldSpace.checkIntersection(entity->getCuboidWorldSpace().getFlatten())) {
        temporaryBatch.push_back(EntityBatchData {
            entity
        });
    }
}

void RenderSystem::batchAppendElevation(Elevation* elevation) {
    /* Collect to temporary vector for further check */
    /* When entering temporaryBatch should be cleared */
    if((elevation->getIndex() > clipElevationIndex) || (batchRenderElevationsProceed == false)) {
        return;
    }

    /* Floor flat tiles */
    for(auto entityIt = elevation->floorEntitiesBegin(); entityIt != elevation->floorEntitiesEnd(); entityIt++) {
        this->temporaryBatchAppendEntity(*entityIt);
    }

    /* Clutter flat tiles */
    for(auto entityIt = elevation->clutterEntitiesBegin(); entityIt != elevation->clutterEntitiesEnd(); entityIt++) {
        this->temporaryBatchAppendEntity(*entityIt);
    }
    
    /* Other big tiles */
    std::vector<EntityBatchData> tmpBatch;
    for(auto entityIt = elevation->biggerEntitiesRegisterBegin(); entityIt != elevation->biggerEntitiesRegisterEnd(); entityIt++) {
        auto entity = *entityIt;
        tmpBatch.push_back(EntityBatchData {
            entity
        });
    }
    std::sort(tmpBatch.begin(), tmpBatch.end());
    temporaryBatch.insert(temporaryBatch.end(), tmpBatch.begin(), tmpBatch.end());

    /* Check for camera attached entity (actually player) to 
    urrent batch intersavion - prevent hiding player behind 
    upper level objects */
    auto observedEntity = camera->getFocusedEntity();
    const auto observedLayerIndex = observedEntity->getContainingElevationOrThrow()->getIndex();

    /* Not apply to layer with player or below */
    if(elevation->getIndex() <= observedLayerIndex) {
        /* For sure visible - move objects to final batch vector */
        enititesBatch.insert(enititesBatch.end(), temporaryBatch.begin(), temporaryBatch.end());
    }

    else {
        /* Above observed entity - check for collision */
        // const auto observedRectElevationSpace = observedEntity->getCuboidElevationSpace()->getFlatten();
        // for(auto tmpEntityData : temporaryBatch) {
        //     auto tmpEntity = tmpEntityData.entity;
        //     const auto tmpRectElevationSpace = tmpEntity->getCuboidElevationSpace()->getFlatten();
            // if(tmpRectElevationSpace.checkIntersection(observedRectElevationSpace)) {
            //     /* Collision - stop rendering elevations */
            //     batchRenderElevationsProceed = false;
            //     break;
            // }
        // }

        /* No collisions - append temporaryBatch */
        enititesBatch.insert(enititesBatch.end(), temporaryBatch.begin(), temporaryBatch.end());
    }
    
    temporaryBatch.clear();
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
        const auto textureRect = textureCmp->getDrawableRectFromWorldSpace();

        if(!textureDataOption.has_value()) {
            /* Corrupted */
            renderFilledRect4F(textureRect, 1.0F, 0.0F, 0.0F);
        }
        else {
            renderTexturedRect4F(textureRect, textureDataOption.value(), textureCmp->getTilesetIndex());
        }
    }

    /* Colored quad rendering */
    auto colorCmpOption = entity->getColorComponent();
    if(colorCmpOption.has_value()) {
        auto colorCmp = colorCmpOption.value();
        //todo colorCmp->a
        renderFilledRect4F(colorCmp->getDrawableRectFromWorldSpace(), colorCmp->r, colorCmp->g, colorCmp->b);
    }
}


void RenderSystem::renderCollisionBoxes(int elevationIndex,
        std::vector<CollisionComponent*>::const_iterator collisionCmpsBegin,
        std::vector<CollisionComponent*>::const_iterator collisionCmpsEnd) {
    for(auto collisionCmpIt = collisionCmpsBegin; collisionCmpIt != collisionCmpsEnd; collisionCmpIt++) {
        auto collisionCmp = *collisionCmpIt;
        const auto collisionCuboids = collisionCmp->getWorldSpaceCollisionCuboids();
        for (const auto& collCuboid : collisionCuboids) {
            renderTranslucentFilledCuboid6F(collCuboid, 0.0F, 0.0F, 1.0F, 0.1F);
        }
    }
}


void RenderSystem::fillScreen(float r, float g, float b, float a) {
    renderTranslucentFilledRect4F(renderBoxWorldSpace, r, g, b, a);
}


