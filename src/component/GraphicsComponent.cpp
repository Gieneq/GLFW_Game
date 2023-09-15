#include "GraphicsComponent.h"
#include "Entity.h"
#include "World.h"
#include "Settings.h"


Rect4F ColorComponent::getDrawableRectFromWorldSpace(bool useProjection) const {
    const auto wolrdCuboid = getElevationCuboid().toWorldSpace().value();
    const Rect4F textureRect{
        wolrdCuboid.x(),
        wolrdCuboid.y() - wolrdCuboid.z() * (useProjection ? Settings::Rendering::Y_PERSPECTIVE_SHIFT : 0.0F),
        wolrdCuboid.w(),
        wolrdCuboid.h()
    };
    return textureRect;
}

ElevationCuboid ColorComponent::getElevationCuboid() const {
    return ElevationCuboid::transformLocalCuboid(parent->getCuboid(), Cuboid6F(&rectRelative));
}


/* Texture Cmp */

Rect4F TextureComponent::getDrawableRectFromWorldSpace(bool useProjection) const {
    const auto wolrdCuboid = getElevationCuboid().toWorldSpace().value();
    const Rect4F textureRect{
        wolrdCuboid.x(),
        wolrdCuboid.y() - wolrdCuboid.z() * (useProjection ? Settings::Rendering::Y_PERSPECTIVE_SHIFT : 0.0F),
        wolrdCuboid.w(),
        wolrdCuboid.h()
    };
    return textureRect;
}

ElevationCuboid TextureComponent::getElevationCuboid() const {
    return ElevationCuboid::transformLocalCuboid(parent->getCuboid(), Cuboid6F(&rectRelative));
}
