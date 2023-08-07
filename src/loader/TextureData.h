#pragma once
#include <string>
#include "TextureId.h"
#include "GLCommon.h"

class TextureData {
public:
    TextureData() = default;
    TextureData(const std::string& absolute_path, int width, int height, int div_width, int div_height, TextureID id, const std::string& name) : absolute_path(absolute_path), width(width), height(height), tileWidth(div_width), tileHeight(div_height), id(id), name(name) {
        tilesPerRow = width / div_width;
    }
    std::string absolute_path{""};
    std::string name{""};
    int width{0};
    int height{0};
    int tileWidth{1};
    int tileHeight{1};
    int tilesPerRow{1};
    TextureID id{};
    
    size_t operator()(const TextureData& textureData) const {
        return id.value;
    }

    bool operator==(const TextureData& other) const {
        return id.value == other.id.value;
    }

    bool operator<(const TextureData& other) const {
        return id.value < other.id.value;
    }

    friend std::ostream& operator<<(std::ostream& os, const TextureData& texture_data) {
        os << "\'" << texture_data.name << "\'" << " (" << texture_data.absolute_path << ") " << texture_data.width << "x" << texture_data.height << " (" << texture_data.tileWidth << "x" << texture_data.tileHeight << ") GPU: " << texture_data.id;
        return os;
    }
};