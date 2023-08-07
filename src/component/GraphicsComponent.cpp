#include "GraphicsComponent.h"
#include "LocationComponent.h"


Rect2F ColorComponent::getWorldRect() {
    return parentLocation->worldRect.get_translated(relativeBoxTranslation);
}

Rect2F TextureComponent::getWorldRect() {
    return parentLocation->worldRect.get_translated(relativeBoxTranslation);
}
