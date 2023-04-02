#pragma once 
#include <GLFW/glfw3.h>
#include <iostream>

class TextureId {
public:
    TextureId() = default;
    TextureId(GLuint id) : id(id) {}
    GLuint id{0};

    bool has_id() const {
        return id != 0;
    }

    size_t operator()(const TextureId& id) const {
        return std::hash<GLuint>()(id.id);
    }
    
    bool operator==(const TextureId& other) const {
        return id == other.id;
    }

    bool operator<(const TextureId& other) const {
        return id < other.id;
    }
    

    friend std::ostream& operator<<(std::ostream& os, const TextureId& id) {
        os << id.id;
        return os;
    }
};
