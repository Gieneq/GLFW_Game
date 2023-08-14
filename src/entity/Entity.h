#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "Component.h"
#include "Maths.h"
#include "TextureID.h"

enum class EntityType {
    NONE,
    FLOOR,
    CLUTTER,
    STATIC,
    DYNAMIC
};

class Elevation;
class ColorComponent;
class TextureComponent;
class MovementComponent;
class CollisionComponent;
class CollisionDetectorComponent;
class ControllableComponent;
class AnimationComponent;
class Entity {
public:
    Entity() = default;
    Entity(Elevation* elevation, EntityType etype) : containingElevation{elevation}, type{etype} {}
    Entity(float elevationSpaceX, float elevationSpaceY, Elevation* elevation, EntityType etype) : cuboidElevationSpace{elevationSpaceX, elevationSpaceY, 0.0F, 1.0F, 1.0F, 0.0F}, containingElevation{elevation}, type{etype} {}
    Entity(float elevationSpaceX, float elevationSpaceY, float elevationSpaceZ, float width, float height, float depth, Elevation* elevation, EntityType etype) :
        cuboidElevationSpace{elevationSpaceX, elevationSpaceY, elevationSpaceZ, width, height, depth}, containingElevation{elevation}, active{true}, type{etype} {}
    
    // todo make Entity responsible for deleting components
    ~Entity() {
        for (auto component : components) {
            delete component;
        }
    }

    bool operator==(const Entity& other) const {
        return getId() == other.getId();
    }

    bool operator!=(const Entity& other) const {
        return getId() != other.getId();
    }

    bool operator>(const Entity& other) const {
        return getId() > other.getId();
    }

    bool operator<(const Entity& other) const {
        return getId() < other.getId();
    }

    inline long long getId() const {
        return id;
    }

    /* Components related methods */

    template<typename T>
    bool hasComponent() const {
        for (auto component : components) {
            T* t = dynamic_cast<T*>(component);
            if (t) {
                return true;
            }
        }
        return false;
    }

    template<typename T>
    T* getComponent() const {
        for (auto component : components) {
            T* t = dynamic_cast<T*>(component);
            if (t) {
                return t;
            }
        }
        return nullptr;
    }
    
    template<typename T>
    std::vector<T*> getComponents() const {
        std::vector<T*> result;
        for (auto component : components) {
            T* t = dynamic_cast<T*>(component);
            if (t) {
                result.push_back(t);
            }
        }
        return result;
    }

    /* Components building methods */
    MovementComponent* addMovementComponent(float speed = 1.0F);
    ColorComponent* addColorComponent(float relX, float relY, float boxWidth, float boxHeight);
    TextureComponent* addTextureComponent(float relX, float relY, float boxWidth, float boxHeight, TextureID id);
    CollisionComponent* addCollisionComponent();
    std::optional<CollisionDetectorComponent*> addCollisionDetectorComponent(const Rect2F& boundingRect);
    std::optional<ControllableComponent*> addControllableComponent();
    std::optional<AnimationComponent*> addAnimationComponent(int interval);
    
    std::optional<ColorComponent*> getColorComponent() const;
    std::optional<TextureComponent*> getTextureComponent() const;
    
    /* Position related methods */

    Rect3F* getCuboidElevationSpace() {
        return &cuboidElevationSpace;
    }
    
    inline Elevation* getContainingElevation() {
        return containingElevation;
    }

    Rect3F getCuboidWorldSpace() const;

protected:
    /* Quick access to popular components */
    ColorComponent* colorComponent{nullptr};
    TextureComponent* textureComponent{nullptr};

private:
    /**
     * ID is ised to distinguish entities.
    */
    long long id{nextID++};
    std::vector<Component*> components;

    /**
     * Active entities can be updated and rendered.
    */
    bool active{false};

    Elevation* containingElevation{nullptr};

    Rect3F cuboidElevationSpace{0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.0F};

    /* General step in adding components */
    void addComponent(Component* component);

    EntityType type{EntityType::NONE};
    
    static long long nextID;
};