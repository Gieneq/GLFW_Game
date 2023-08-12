#include "GraphicsComponent.h"
#include "Entity.h"


Vect2F ColorComponent::getPositionElevationSpace() const {
    return parent->getPositionElevationSpace().getTranslated(relativeBoxTranslation);
}

Rect2F ColorComponent::getRectElevationSpace() const {
    return Rect2F{getPositionElevationSpace(), size};
}

Vect3F ColorComponent::getPositionWorldSpace() const {
    return Vect3F{getPositionElevationSpace(), parent->getWorldSpaceZ()};
}

/* Texture Cmp */



Vect2F TextureComponent::getPositionElevationSpace() const {
    return parent->getPositionElevationSpace().getTranslated(relativeBoxTranslation);
}

Rect2F TextureComponent::getRectElevationSpace() const {
    return Rect2F{getPositionElevationSpace(), size};
}

Vect3F TextureComponent::getPositionWorldSpace() const {
    return Vect3F{getPositionElevationSpace(), parent->getWorldSpaceZ()};
}