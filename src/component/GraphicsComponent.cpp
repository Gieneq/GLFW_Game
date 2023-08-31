#include "GraphicsComponent.h"
#include "Entity.h"
#include "World.h"



Rect4F ColorComponent::getDrawableRectFromWorldSpace() const {
    const auto wolrdCuboid = getCuboidWorldSpace();
    const Rect4F textureRect{
        wolrdCuboid.topLeft.x,
        wolrdCuboid.topLeft.y - wolrdCuboid.topLeft.z,
        wolrdCuboid.size.w,
        wolrdCuboid.size.h
    };
    return textureRect;
}

Cuboid6F ColorComponent::getCuboidElevationSpace() const {
    const auto parentEleveationSpacePosition = parent->getCuboidElevationSpace().topLeft;

    return Cuboid6F{
        rectRelative.x() + parentEleveationSpacePosition.x,
        rectRelative.y() + parentEleveationSpacePosition.y,
        0.0F + parentEleveationSpacePosition.z,
        rectRelative.w(),
        rectRelative.h(),
        0.0F
    };  
}

Cuboid6F ColorComponent::getCuboidWorldSpace() const {
    const auto parentWorldSpacePosition = parent->getCuboidWorldSpace().topLeft;

    return Cuboid6F{
        rectRelative.x() + parentWorldSpacePosition.x,
        rectRelative.y() + parentWorldSpacePosition.y,
        0.0F + parentWorldSpacePosition.z,
        rectRelative.w(),
        rectRelative.h(),
        0.0F
    };  
}


/* Texture Cmp */

Rect4F TextureComponent::getDrawableRectFromWorldSpace() const {
    const auto wolrdCuboid = getCuboidWorldSpace();
    const Rect4F textureRect{
        wolrdCuboid.topLeft.x,
        wolrdCuboid.topLeft.y - wolrdCuboid.topLeft.z,
        wolrdCuboid.size.w,
        wolrdCuboid.size.h
    };
    return textureRect;
}

Cuboid6F TextureComponent::getCuboidElevationSpace() const {
    const auto parentEleveationSpacePosition = parent->getCuboidElevationSpace().topLeft;

    return Cuboid6F{
        rectRelative.x() + parentEleveationSpacePosition.x,
        rectRelative.y() + parentEleveationSpacePosition.y,
        0.0F + parentEleveationSpacePosition.z,
        rectRelative.w(),
        rectRelative.h(),
        0.0F
    };  
}

Cuboid6F TextureComponent::getCuboidWorldSpace() const {
    const auto parentWorldSpacePosition = parent->getCuboidWorldSpace().topLeft;

    return Cuboid6F{
        rectRelative.x() + parentWorldSpacePosition.x,
        rectRelative.y() + parentWorldSpacePosition.y,
        0.0F + parentWorldSpacePosition.z,
        rectRelative.w(),
        rectRelative.h(),
        0.0F
    };  
}
