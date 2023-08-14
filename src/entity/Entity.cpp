#include "Entity.h"
#include "World.h"
#include "GraphicsComponent.h"
#include "MovementComponent.h"
#include "CollisionComponents.h"
#include "AnimationComponent.h"
#include "ControllableComponent.h"

long long Entity::nextID = 0;


void Entity::addComponent(Component* component) {
    components.push_back(component);

    if (auto colorCmp = dynamic_cast<ColorComponent*>(component)) {
        colorComponent = colorCmp;
    }
    else if (auto textureCmp = dynamic_cast<TextureComponent*>(component)) {
        textureComponent = textureCmp;
    }
}

/* Components builders */

MovementComponent* Entity::addMovementComponent(float speed) {
    auto movementComponent = new MovementComponent(this);
    movementComponent->speed = speed;
    movementComponent->setDirection(0.0F, 1.0F, 0.0F);
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


Rect3F Entity::getCuboidWorldSpace() const {
    return Rect3F(
        cuboidElevationSpace.topLeft.x + 0.0F,
        cuboidElevationSpace.topLeft.y +  0.0F,
        cuboidElevationSpace.topLeft.z + containingElevation->getWorldSpaceZ(),
        cuboidElevationSpace.size.w,
        cuboidElevationSpace.size.h,
        cuboidElevationSpace.size.d
    );
}