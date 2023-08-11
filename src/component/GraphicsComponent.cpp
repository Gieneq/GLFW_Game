#include "GraphicsComponent.h"
#include "Entity.h"


Vect2F ColorComponent::getPositionElevationSpace() const {
    return parent->getPositionElevationSpace().getTranslated(relativeBoxTranslation);
}


Vect3F ColorComponent::getPositionWorldSpace() const {
    return Vect3F{getPositionElevationSpace(), parent->getWorldSpaceZ()};
}

Vect2F TextureComponent::getPositionElevationSpace() const {
    return parent->getPositionElevationSpace().getTranslated(relativeBoxTranslation);
}

Vect3F TextureComponent::getPositionWorldSpace() const {
    return Vect3F{getPositionElevationSpace(), parent->getWorldSpaceZ()};
}