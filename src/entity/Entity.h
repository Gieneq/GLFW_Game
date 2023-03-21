#pragma once
#include <iostream>
#include <vector>
#include "../component/Component.h"

class Entity {
public:
    void addComponent(Component* component);
    // void removeComponent(Component* component);

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
private:
    std::vector<Component*> components;
};