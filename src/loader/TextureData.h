#pragma once
#include <string>
#include "TextureId.h"
#include "GLCommon.h"

class TextureData {
public:
    TextureData() = default;
    TextureData(const std::string& absolute_path, int width, int height, TextureId id, const std::string& name) : absolute_path(absolute_path), width(width), height(height), id(id), name(name) {}
    std::string absolute_path{""};
    std::string name{""};
    int width{0};
    int height{0};
    int div_width{1};
    int div_height{1};
    TextureId id{};
    
    size_t operator()(const TextureData& textureData) const {
        return id.id;
    }

    bool operator==(const TextureData& other) const {
        return id.id == other.id.id;
    }

    bool operator<(const TextureData& other) const {
        return id.id < other.id.id;
    }

    friend std::ostream& operator<<(std::ostream& os, const TextureData& texture_data) {
        os << "TextureData: " << texture_data.name << " (" << texture_data.absolute_path << ") " << texture_data.width << "x" << texture_data.height << " (" << texture_data.div_width << "x" << texture_data.div_height << ") " << texture_data.id;
        return os;
    }
};