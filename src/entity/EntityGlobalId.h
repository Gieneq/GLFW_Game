#pragma once 
#include <string>
#include <iostream>

class EntityGlobalId {
public:
    EntityGlobalId() = default;
    EntityGlobalId(int id) : id(id) {}
    int id{-1};

    size_t operator()(const EntityGlobalId& id) const {
        return std::hash<int>()(id.id);
    }
    
    bool operator==(const EntityGlobalId& other) const {
        return id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const EntityGlobalId& id) {
        os << id.id;
        return os;
    }
};
