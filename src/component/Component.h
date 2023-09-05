#pragma once
#include <iostream>

class Entity;

class Component {
public:
    virtual ~Component() = default;
    Component(Entity* parent) : parent{parent} {}

    inline Entity* getParentEntity() {
        return parent;
    }

    template <typename Cmp>
    static typename std::vector<Cmp*> getComponents(const std::vector<Entity*>& entities) {
        std::vector<Cmp*> result;
        for (auto entity : entities) {
            const std::vector<Cmp*> cmps = entity->getComponents<Cmp>();
            if (cmp.size() > 0) {
                result.insert(result.end(), cmps.begin(), cmps.end());
            }
        }
        return result;
    }
    
protected:
    Entity* parent{nullptr};
};

template <typename T>
struct ComponentName {
    static std::string get() {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        return "UnknownComponent";
    }
};