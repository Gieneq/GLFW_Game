#include "GraphicsComponent.h"
#include "Entity.h"
#include "World.h"

Rect4F ColorComponent::getRectElevationSpace() const {
    return Rect4F{
        rectRelative.topLeft.getTranslated(parent->getCuboidElevationSpace().topLeft.getXY()),
        rectRelative.size
    };
}

Rect6F ColorComponent::getCubiodWorldSpace() const {
    auto rectElevationSpace = getRectElevationSpace();
    return Rect6F{
        rectElevationSpace.topLeft.x,
        rectElevationSpace.topLeft.y,
        parent->getContainingElevationOrThrow()->getWorldSpaceZ(),
        rectElevationSpace.size.w,
        rectElevationSpace.size.h,
        0.0F
    };  
}

/* Texture Cmp */

Rect4F TextureComponent::getRectElevationSpace() const {
    return Rect4F{
        rectRelative.topLeft.getTranslated(parent->getCuboidElevationSpace().topLeft.getXY()),
        rectRelative.size
    };
}

Rect6F TextureComponent::getCubiodWorldSpace() const {
    auto rectElevationSpace = getRectElevationSpace();
    return Rect6F{
        rectElevationSpace.topLeft.x,
        rectElevationSpace.topLeft.y,
        parent->getContainingElevationOrThrow()->getWorldSpaceZ(),
        rectElevationSpace.size.w,
        rectElevationSpace.size.h,
        0.0F
    };  
}
