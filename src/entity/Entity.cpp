#include "Entity.h"
#include "World.h"
#include "GraphicsComponent.h"
#include "MovementComponent.h"
#include "CollisionComponents.h"
#include "AnimationComponent.h"
#include "ControllableComponent.h"

long long Entity::nextID = 0;

float Entity::getWorldSpaceZ() const {
    return containingElevation->getWorldSpaceZ();
}

void Entity::addComponent(Component* component) {
    components.push_back(component);

    if (auto colorCmp = dynamic_cast<ColorComponent*>(component)) {
        colorComponent = colorCmp;
    }
    else if (auto textureCmp = dynamic_cast<TextureComponent*>(component)) {
        textureComponent = textureCmp;
    }
}

Vect3F Entity::getPositionWorldSpace() const {
    return Vect3F{positionElevationSpace, getWorldSpaceZ()};
}

/* Components builders */

MovementComponent* Entity::addMovementComponent(float speed) {
    auto movementComponent = new MovementComponent(this);
    movementComponent->speed = speed;
    movementComponent->setDirection(Direction::SOUTH);
    addComponent(movementComponent);
    return movementComponent;
}

ColorComponent* Entity::addColorComponent(float relX, float relY, float boxWidth, float boxHeight) {
    auto colorComponent = new ColorComponent(this, relX, relY, boxWidth, boxHeight);
    addComponent(colorComponent);
    return colorComponent;
}

TextureComponent* Entity::addTextureComponent(float relX, float relY, float boxWidth, float boxHeight, TextureID id) {
    auto textureComponent = new TextureComponent(this, relX, relY, boxWidth, boxHeight, id);
    addComponent(textureComponent);
    return textureComponent;
}

CollisionComponent* Entity::addCollisionComponent() {
    auto collisionComponent = new CollisionComponent(this);
    addComponent(collisionComponent);
    return collisionComponent;
}

std::optional<CollisionDetectorComponent*> Entity::addCollisionDetectorComponent(const Rect2F& boundingRect) {
    auto movementCmp = getComponent<MovementComponent>();
    if(!movementCmp) {
        return std::nullopt;
    }
    
    /* Only one can be applied */
    if (hasComponent<CollisionDetectorComponent>()) {
        return std::nullopt;
    }

    auto collisionDetectorComponent = new CollisionDetectorComponent(this, movementCmp, boundingRect);
    addComponent(collisionDetectorComponent);
    return collisionDetectorComponent;
}

std::optional<ControllableComponent*> Entity::addControllableComponent() {
    auto movementCmp = getComponent<MovementComponent>();
    if(!movementCmp) {
        return std::nullopt;
    }

    /* Only one can be applied */
    if (hasComponent<ControllableComponent>()) {
        return std::nullopt;
    }

    auto controllableComponent = new ControllableComponent(this, movementCmp);
    addComponent(controllableComponent);
    return controllableComponent;
}

std::optional<AnimationComponent*> Entity::addAnimationComponent(int interval) {
    auto textureCmp = getComponent<TextureComponent>();
    if(!textureCmp) {
        return std::nullopt;
    }

    /* Only one can be applied */
    if (hasComponent<AnimationComponent>()) {
        return std::nullopt;
    }

    auto animationComponent = new AnimationComponent(this, textureCmp, interval);
    addComponent(animationComponent);
    return animationComponent;
}

std::optional<ColorComponent*> Entity::getColorComponent() const {
    if(!colorComponent) {
        return std::nullopt;
    }
    return colorComponent;
}

std::optional<TextureComponent*> Entity::getTextureComponent() const {
    if(!textureComponent) {
        return std::nullopt;
    }
    return textureComponent;
}

Rect2F Entity::getRectElevationSpace() const {
    return Rect2F{
        positionElevationSpace.x,
        positionElevationSpace.y,
        size.w,
        size.h
    };
}
Rect3F Entity::getBoxElevationSpace() const {
    auto box = Rect3F{
        positionElevationSpace.x,
        positionElevationSpace.y,
        heightElevationSpace,
        size.w,
        size.h,
        length
    };
    return box;
}

Rect3F Entity::getBoxWorldSpace() const{
    auto box = getBoxElevationSpace();
    box.z() += getWorldSpaceZ();
    return box;
}