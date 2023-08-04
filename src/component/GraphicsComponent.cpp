#include "GraphicsComponent.h"
#include "LocationComponent.h"


Rect2F ColorComponent::getWorldRect() {
    return relativeBoxRect.get_translated(parentLocation->worldRect.top_left);
}

Rect2F TextureComponent::getWorldRect() {
    return relativeBoxRect.get_translated(parentLocation->worldRect.top_left);
}
