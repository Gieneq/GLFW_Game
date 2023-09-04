#include "GraphicsComponent.h"
#include "Entity.h"
#include "World.h"



Rect4F ColorComponent::getDrawableRectFromWorldSpace() const {
    const auto wolrdCuboid = getElevationCuboid().toWorldSpace().value();
    const Rect4F textureRect{
        wolrdCuboid.x(),
        wolrdCuboid.y() - wolrdCuboid.z(),
        wolrdCuboid.w(),
        wolrdCuboid.h()
    };
    return textureRect;
}

ElevationCuboid ColorComponent::getElevationCuboid() const {
    return ElevationCuboid::transformLocalCuboid(parent->getCuboid(), Cuboid6F(&rectRelative));
}


/* Texture Cmp */

Rect4F TextureComponent::getDrawableRectFromWorldSpace() const {
    const auto wolrdCuboid = getElevationCuboid().toWorldSpace().value();
    const Rect4F textureRect{
        wolrdCuboid.x(),
        wolrdCuboid.y() - wolrdCuboid.z(),
        wolrdCuboid.w(),
        wolrdCuboid.h()
    };
    return textureRect;
}

ElevationCuboid TextureComponent::getElevationCuboid() const {
    return ElevationCuboid::transformLocalCuboid(parent->getCuboid(), Cuboid6F(&rectRelative));
}
