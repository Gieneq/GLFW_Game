#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include "Component.h"
#include "Maths.h"
#include "Coordinates.h"
#include "TextureID.h"

enum class EntityType {
    NONE,
    FLOOR,
    CLUTTER,
    STATIC,
    DYNAMIC,
    BORDER,
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
protected:
    // Entity() = default;
    Entity(Elevation* elevation, EntityType etype) : containingElevation{elevation}, type{etype}, cuboid{reinterpret_cast<ElevationDepth*>(elevation), 0,0,0,1,1,1} {}

public:
    // todo make Entity responsible for deleting components - IMPORTANT!
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

    inline EntityType getType() const {
        return type;
    } 

    Elevation* getContainingElevationOrThrow() const;

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

    /* Components building methods */
    MovementComponent* addMovementComponent(float speed = 1.0F);

    ColorComponent* addColorComponent(float relX = 0.0F, float relY = 0.0F, float boxWidth = 1.0F, float boxHeight = 1.0F);

    TextureComponent* addTextureComponent(TextureID id, float relX = 0.0F, float relY = 0.0F, float boxWidth = 1.0F, float boxHeight = 1.0F);

    CollisionComponent* addCollisionComponent();

    std::optional<CollisionDetectorComponent*> addCollisionDetectorComponent(const Cuboid6F& boundingCuboid);

    std::optional<ControllableComponent*> addControllableComponent();

    std::optional<AnimationComponent*> addAnimationComponent(int interval);
    
    /* Access components */
    std::optional<ColorComponent*> getColorComponent() const;

    std::optional<TextureComponent*> getTextureComponent() const;
    
    /* Position related methods */

    inline ElevationCuboid& getCuboid() {
        return cuboid;
    }

    inline const ElevationCuboid& getCuboid() const {
        return cuboid;
    }

protected:
    /* Quick access to popular components */
    ColorComponent* colorComponent{nullptr};

    TextureComponent* textureComponent{nullptr};

private:
    void setElevation(Elevation* elevation);
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

    ElevationCuboid cuboid{nullptr, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.0F};

    /* General step in adding components */
    template<typename T>
    void addComponent(Component* component) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

        if(hasComponent<T>()) {
            throw std::runtime_error("Entity::addMovementComponent: Component already exists");
        }

        components.push_back(component);

        if (auto colorCmp = dynamic_cast<ColorComponent*>(component)) {
            colorComponent = colorCmp;
        }
        else if (auto textureCmp = dynamic_cast<TextureComponent*>(component)) {
            textureComponent = textureCmp;
        }
    }

    EntityType type{EntityType::NONE};
    
    static long long nextID;

    friend class Loader;
    friend class Elevation;
};