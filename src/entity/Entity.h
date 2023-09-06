#pragma once
#include <iostream>
#include <optional>
#include <vector>
#include <array>
#include "Component.h"
#include "Maths.h"
#include "Coordinates.h"
#include "TextureID.h"

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
    enum class Type {
        NONE,
        FLOOR,
        CLUTTER,
        STATIC,
        DYNAMIC,
        BORDER,
    };

    static bool isBiggerType(Type entityType) {
        return entityType == Type::STATIC || entityType == Type::DYNAMIC;
    }

    static constexpr const char* typeToString(Type type) {
        switch (type) {
            case Type::NONE:
                return "NONE";
            case Type::FLOOR:
                return "FLOOR";
            case Type::CLUTTER:
                return "CLUTTER";
            case Type::STATIC:
                return "STATIC";
            case Type::DYNAMIC:
                return "DYNAMIC";
            case Type::BORDER:
                return "BORDER";
            default:
                return "UNKNOWN";
        }
    }

    static constexpr std::array<Type, 6> AllTypes = {
        Type::NONE,
        Type::FLOOR,
        Type::CLUTTER,
        Type::STATIC,
        Type::DYNAMIC,
        Type::BORDER
    };

    static constexpr auto TypesCount = AllTypes.size();

protected:
    Entity(Elevation* elevation, Entity::Type etype) : containingElevation{elevation}, 
        type{etype}, cuboid{reinterpret_cast<ElevationDepth*>(elevation), 0,0,0,1,1,1} {}
   
    ~Entity() {
        for (auto component : components) {
            delete component;
        }
    }

public:
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

    inline Entity::Type getType() const {
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
    virtual MovementComponent* addMovementComponent(float speed = 1.0F);

    ColorComponent* addColorComponent(float relX = 0.0F, float relY = 0.0F, float boxWidth = 1.0F, float boxHeight = 1.0F);

    TextureComponent* addTextureComponent(TextureID id, float relX = 0.0F, float relY = 0.0F, float boxWidth = 1.0F, float boxHeight = 1.0F);

    CollisionComponent* addCollisionComponent();

    virtual std::optional<CollisionDetectorComponent*> addCollisionDetectorComponent(const Cuboid6F& boundingCuboid);

    virtual std::optional<ControllableComponent*> addControllableComponent();

    std::optional<AnimationComponent*> addAnimationComponent(int interval);
    
    /* Access components */
    std::optional<ColorComponent*> getColorComponent() const;

    std::optional<TextureComponent*> getTextureComponent() const;
    
    /* Position related member functions */
    inline ElevationCuboid& getCuboid() {
        return cuboid;
    }

    inline const ElevationCuboid& getCuboid() const {
        return cuboid;
    }

protected:
    /**
     * Quick access to popular components.
     * Entity can has up to one component of each type.
     */
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

    /**
     * General step in adding components.
     * Esures only up to one component of each type is added.
     */
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

    Entity::Type type{Entity::Type::NONE};
    
    static long long nextID;

    friend class Elevation;
    friend class Loader;
};