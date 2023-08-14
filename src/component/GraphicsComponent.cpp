#include "GraphicsComponent.h"
#include "Entity.h"
#include "World.h"

Rect2F ColorComponent::getRectElevationSpace() const {
    return Rect2F{
        rectRelative.topLeft.getTranslated(parent->getCuboidElevationSpace()->topLeft.getXY()),
        rectRelative.size
    };
}

Rect3F ColorComponent::getCubiodWorldSpace() const {
    auto rectElevationSpace = getRectElevationSpace();
    return Rect3F{
        rectElevationSpace.topLeft.x,
        rectElevationSpace.topLeft.y,
        parent->getContainingElevation()->getWorldSpaceZ(),
        rectElevationSpace.size.w,
        rectElevationSpace.size.h,
        0.0F
    };  
}

/* Texture Cmp */

Rect2F TextureComponent::getRectElevationSpace() const {
    return Rect2F{
        rectRelative.topLeft.getTranslated(parent->getCuboidElevationSpace()->topLeft.getXY()),
        rectRelative.size
    };
}

Rect3F TextureComponent::getCubiodWorldSpace() const {
    auto rectElevationSpace = getRectElevationSpace();
    return Rect3F{
        rectElevationSpace.topLeft.x,
        rectElevationSpace.topLeft.y,
        parent->getContainingElevation()->getWorldSpaceZ(),
        rectElevationSpace.size.w,
        rectElevationSpace.size.h,
        0.0F
    };  
}
