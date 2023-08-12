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
    Entity(Elevation* elevation) : containingElevation{elevation} {}
    Entity(float elevationSpaceX, float elevationSpaceY, Elevation* elevation) : positionElevationSpace{elevationSpaceX, elevationSpaceY}, containingElevation{elevation} {}
    Entity(float elevationSpaceX, float elevationSpaceY, float elevationSpaceZ, float width, float height, float length, Elevation* elevation) :
        positionElevationSpace{elevationSpaceX, elevationSpaceY}, heightElevationSpace{elevationSpaceZ}, size{width, height}, length{length}, 
        containingElevation{elevation}, active{true} {}
    
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
    float getWorldSpaceZ() const;

    Vect3F getPositionWorldSpace() const;

    Vect2F& getPositionElevationSpace() {
        return positionElevationSpace;
    }

    inline Size2F getSize() const {
        return size;
    }

    inline void setSize(float w, float h) {
        size.w = w;
        size.h = h;
    }

    inline float getLength() const {
        return length;
    }

    inline void setLength(float len) {
        length = len;
    }
    
    inline Elevation* getContainingElevation() {
        return containingElevation;
    }

    inline float getLeftElevationSpace() const {
        return positionElevationSpace.x;
    }

    inline float getRightElevationSpace() const {
        return positionElevationSpace.x + size.w;
    }

    inline float getTopElevationSpace() const {
        return positionElevationSpace.y;
    }

    inline float getBottomElevationSpace() const {
        return positionElevationSpace.y + size.h;
    }

    inline float getDownElevationSpace() const {
        return heightElevationSpace;
    }

    inline float getUpElevationSpace() const {
        return heightElevationSpace + length;
    }

    inline void setXElevationSpace(float x) {
        positionElevationSpace.x = x;
    }

    inline void setYElevationSpace(float y) {
        positionElevationSpace.y = y;
    }

    inline void setZElevationSpace(float z) {
        heightElevationSpace = z;
    }

        inline void addXElevationSpace(float dx) {
        positionElevationSpace.x += dx;
    }

    inline void addYElevationSpace(float dy) {
        positionElevationSpace.y += dy;
    }

    inline void addZElevationSpace(float dz) {
        heightElevationSpace += dz;
    }

    Rect2F getRectElevationSpace() const;
    Rect3F getBoxElevationSpace() const;
    Rect3F getBoxWorldSpace() const;

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

    /**
     * Size 1x1 is base tilesize.
     * Length 1 is space between layers.
    */
    Size2F size{1.0F, 1.0F};
    float length{0.0F};

    /**
     * Position of top_left corner in relation to containing Elevation.
     * Height can be used to place object on top of another.
    */
    Vect2F positionElevationSpace{0.0F, 0.0F};
    float heightElevationSpace{0.0F};

    /* General step in adding components */
    void addComponent(Component* component);
    
    static long long nextID;
};