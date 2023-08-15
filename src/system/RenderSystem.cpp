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

void RenderSystem::temporaryBatchAppendEntity(Entity* e) {
    if(renderBoxWorldSpace.checkIntersection(e->getCuboidWorldSpace().getFlatten())) {
        temporaryBatch.push_back(EntityBatchData {
            e, 
            e->getCuboidElevationSpace()->bottom()
        });
    }
}

void RenderSystem::batchAppendElevation(Elevation* elevation) {
    /* Collect to temporary vector for further check */
    /* When entering temporaryBatch should be cleared */
    if(elevation->getIndex() > 5 || elevation->getIndex() < 0) {
        int err1 = 1;
    }

    if(elevation->getIndex() > clipElevationIndex) {
        return;
    }

    if(batchRenderElevationsProceed == false) {
        return;
    }

    /* Floor flat tiles */
    for(auto entity: elevation->getFloorEntities()) {
        this->temporaryBatchAppendEntity(entity);
    }

    /* Clutter flat tiles */
    for(auto entity: elevation->getClutterEntities()) {
        this->temporaryBatchAppendEntity(entity);
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
    temporaryBatch.insert(temporaryBatch.end(), tmpBatch.begin(), tmpBatch.end());

    /* Check for camera attached entity (actually player) to 
    urrent batch intersavion - prevent hiding player behind 
    upper level objects */
    auto observedEntity = camera->getFocusedEntity();
    const auto observedLayerIndex = observedEntity->getContainingElevation()->getIndex();

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
        if(!textureDataOption.has_value()) {
            /* Corrupted */
            renderFilledBox(textureCmp->getRectElevationSpace(), 1.0F, 0.0F, 0.0F);
        }
        else {
            Rect2F textureRect = textureCmp->getRectElevationSpace();
            auto currentElevation = entity->getContainingElevation();
            auto elevationIndex = currentElevation->getWorldSpaceZ();
            if(elevationIndex > 5 || elevationIndex < 0) {
                std::cout << textureRect <<  ", ei: " << elevationIndex << ", cuboid: " << entity->getCuboidWorldSpace() << std::endl;
            }
            textureRect.topLeft.y -= elevationIndex;
            renderTexturedBox(textureRect, textureDataOption.value(), textureCmp->getTilesetIndex());
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




