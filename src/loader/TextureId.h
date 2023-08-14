#pragma once 
#include "GLCommon.h"
#include <iostream>

class TextureData;
class TextureID {
public:
    TextureID() = default;
    TextureID(GLuint id) : value(id) {}

    bool hasID() const {
        return value != 0;
    }

    GLuint getID() const {
        return value;
    }

    TextureData* getAssosiatedTextureData() const {
        return assosiatedTextureData;
    }

    size_t operator()(const TextureID& id) const {
        return std::hash<GLuint>()(id.value);
    }
    
    bool operator==(const TextureID& other) const {
        return value == other.value;
    }

    bool operator<(const TextureID& other) const {
        return value < other.value;
    }

    friend std::ostream& operator<<(std::ostream& os, const TextureID& id) {
        os << id.value;
        return os;
    }
    
private:
    GLuint value{0};
    TextureData* assosiatedTextureData{nullptr};

    friend class TextureData;
    friend class LoaderBase;
};
