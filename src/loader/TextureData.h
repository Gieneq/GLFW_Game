#pragma once
#include <string>
#include "TextureId.h"
#include "GLCommon.h"

class TextureData {
public:
    TextureData() = default;
    TextureData(const std::string& absolute_path, int width, int height, const GLuint gl_id) : absolute_path(absolute_path), width(width), height(height), id(gl_id) {}
    std::string absolute_path{""};
    int width{0};
    int height{0};
    TextureId id{};
    static TextureData corrupted() {
        return TextureData("", 0, 0, 0);
    }
};