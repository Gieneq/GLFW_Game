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

    bool operator==(const Component& other) const {
        return parent == other.parent;
    }

    bool operator!=(const Component& other) const {
        return !(*this == other);
    }

    bool operator<(const Component& other) const {
        return parent < other.parent;
    }

    bool operator>(const Component& other) const {
        return parent > other.parent;
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
    
protected:
    Entity* parent{nullptr};

};
