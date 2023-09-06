#include "Entity.h"
#include "World.h"
#include "GraphicsComponent.h"
#include "MovementComponent.h"
#include "CollisionComponents.h"
#include "AnimationComponent.h"
#include "ControllableComponent.h"

long long Entity::nextID = 0;

Elevation* Entity::getContainingElevationOrThrow() const {
    if(containingElevation == nullptr) {
        throw std::runtime_error("Entity::getContainingElevationOrThrow: containingElevation is nullptr");
    }
    return containingElevation;
}

/* Components creators */

MovementComponent* Entity::addMovementComponent(float speed) {
    /* Dependencies - none */

    /* Creation and registration */
    auto movementComponent = new MovementComponent(this);
    movementComponent->speed = speed;
    movementComponent->stop();
    addComponent<MovementComponent>(movementComponent);

    /* Register movement component in containing Elevation */
    containingElevation->regiesterComponentsOfEntity(this);

    return movementComponent;
}

ColorComponent* Entity::addColorComponent(float relX, float relY, float boxWidth, float boxHeight) {
    /* Dependencies - none */
    
    /* Creation and registration */
    auto colorComponent = new ColorComponent(this, relX, relY, boxWidth, boxHeight);
    addComponent<ColorComponent>(colorComponent);
    
    /* Register in quick access */
    this->colorComponent = colorComponent;
    return colorComponent;
}

TextureComponent* Entity::addTextureComponent(TextureID id, float relX, float relY, float boxWidth, float boxHeight) {
    /* Dependencies - none */

    /* Creation and registration */
    auto textureComponent = new TextureComponent(this, relX, relY, boxWidth, boxHeight, id);
    addComponent<TextureComponent>(textureComponent);
    
    /* Register in quick access */
    this->textureComponent = textureComponent;
    return textureComponent;
}

CollisionComponent* Entity::addCollisionComponent() {
    /* Dependencies - none */

    /* Creation and registration */
    auto collisionComponent = new CollisionComponent(this);
    addComponent<CollisionComponent>(collisionComponent);

    /* Register collision component in containing Elevation */
    containingElevation->regiesterComponentsOfEntity(this);
    return collisionComponent;
}

std::optional<CollisionDetectorComponent*> Entity::addCollisionDetectorComponent(const Cuboid6F& boundingCuboid) {
    /* Dependencies */
    auto movementCmp = getComponent<MovementComponent>();
    if(!movementCmp) {
        return std::nullopt;
    }

    /* Creation and registration */
    auto collisionDetectorComponent = new CollisionDetectorComponent(this, movementCmp, boundingCuboid);
    addComponent<CollisionDetectorComponent>(collisionDetectorComponent);
    return collisionDetectorComponent;
}

std::optional<ControllableComponent*> Entity::addControllableComponent() {
    /* Dependencies */
    auto movementCmp = getComponent<MovementComponent>();
    if(!movementCmp) {
        return std::nullopt;
    }

    /* Creation and registration */
    auto controllableComponent = new ControllableComponent(this, movementCmp);
    addComponent<ControllableComponent>(controllableComponent);
    return controllableComponent;
}

std::optional<AnimationComponent*> Entity::addAnimationComponent(int interval) {
    /* Dependencies */
    auto textureCmp = getComponent<TextureComponent>();
    if(!textureCmp) {
        return std::nullopt;
    }

    /* Creation and registration */
    auto animationComponent = new AnimationComponent(this, textureCmp, interval);
    addComponent<AnimationComponent>(animationComponent);
    return animationComponent;
}

std::optional<ColorComponent*> Entity::getColorComponent() const {
    return !colorComponent ? std::nullopt : std::make_optional(colorComponent);
}

std::optional<TextureComponent*> Entity::getTextureComponent() const {
    return !textureComponent ? std::nullopt : std::make_optional(textureComponent);
}

/* Should be used instead member variable access */
void Entity::setElevation(Elevation* elevation) {
    this->containingElevation = elevation;
    this->cuboid.setElevationDepth(elevation);
}