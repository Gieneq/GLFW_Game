#pragma once
#include <iostream>
#include <vector>

class Entity;

class Component {
public:
    virtual ~Component() = default;
    Component(Entity* parent) : parent{parent} {}

    inline Entity* getParentEntity() {
        return parent;
    }
    
    template <typename Cmp>
    static std::vector<Cmp*> getComponents(std::vector<Entity*>::const_iterator begin, std::vector<Entity*>::const_iterator end) {
        std::vector<Cmp*> result;
        for (auto it = begin; it != end; ++it) {
            /* Assume - each entity has up to 1 component of each type */
            Cmp* cmp = (*it)->getComponent<Cmp>();
            if (cmp) {
                result.push_back(cmp);
            }
        }
        return result;
    }

    template <typename Cmp>
    static int getComponentId() {
        static_assert(std::is_base_of<Component, Cmp>::value, "Cmp must be a Component");
        static int id = nextId++;
        return id;
    }

    static int getId() {
        return id;
    }

    
protected:
    Entity* parent{nullptr};

private:
    static int id;
    static int nextId;
};

int Component::id = 0;
int Component::nextId = 0;

// template <typename T>
// struct ComponentName {
//     static std::string get() {
//         static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
//         return "UnknownComponent";
//     }
// };