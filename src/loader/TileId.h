#pragma once

#include <string>
#include <iostream>

class TileId {
public:
    TileId() = default;
    TileId(int id) : id(id) {}
    int id{-1};

    size_t operator()(const TileId& id) const {
        return std::hash<int>()(id.id);
    }
    
    bool operator==(const TileId& other) const {
        return id == other.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const TileId& id) {
        os << id.id;
        return os;
    }
};
