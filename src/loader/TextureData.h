#pragma once
#include <string>
#include "TextureId.h"
#include "GLCommon.h"
#include "Settings.h"
#include "Maths.h"

class TextureData {
public:
    const std::string& getAbsolutePath() const {
        return absolute_path;
    }

    const std::string& getName() const {
        return name;
    }

    int getImageWidth() const {
        return imageWidth;
    }

    int getImageHeight() const {
        return imageHeight;
    }

    int getTileWidth() const {
        return tileWidth;
    }

    int getTileHeight() const {
        return tileHeight;
    }

    int getRows() const {
        return rows;
    }

    int getColumns() const {
        return columns;
    }

    int getTilesCount() const {
        return tilesCount;
    }

    TextureID getTextureID() {
        return id;
    }

    size_t operator()(const TextureData& textureData) const {
        return id.getID();
    }

    bool operator==(const TextureData& other) const {
        return id.getID() == other.id.getID();
    }

    bool operator<(const TextureData& other) const {
        return id.getID() < other.id.getID();
    }

    friend std::ostream& operator<<(std::ostream& os, const TextureData& texture_data) {
        os << "\'" << texture_data.name << "\'" << " (" << texture_data.absolute_path << ") " << texture_data.getImageWidth() << "x" << texture_data.getImageHeight() << " (" << texture_data.getTileWidth() << ", " << texture_data.getTileHeight() << ") GPU: " << texture_data.id;
        return os;
    }

    Rect4F getUV(const int tilesetIndex) {
        const int u_idx = tilesetIndex % this->getColumns();
        const int v_idx = tilesetIndex / this->getColumns();

        const int tileLeftPx = Settings::Rendering::Bleeding::MARGIN + u_idx * (Settings::Rendering::Bleeding::SPACING + this->getTileWidth());
        const int tileRightPx = tileLeftPx + this->getTileWidth();
        const int tileTopPx = Settings::Rendering::Bleeding::MARGIN + v_idx * (Settings::Rendering::Bleeding::SPACING + this->getTileHeight());
        const int tileBottomPx = tileTopPx + this->getTileHeight();

        const float u1 = static_cast<float>(tileLeftPx) / static_cast<float>(this->getImageWidth());
        const float u2 = static_cast<float>(tileRightPx) / static_cast<float>(this->getImageWidth());
        const float v1 = static_cast<float>(tileTopPx) / static_cast<float>(this->getImageHeight());
        const float v2 = static_cast<float>(tileBottomPx) / static_cast<float>(this->getImageHeight());

        return Rect4F(u1, v1, u2-u1, v2-v1);
    }

private:    
    std::string absolute_path{""};
    std::string name{""};
    int imageWidth{300};
    int imageHeight{300};
    int tileWidth{20};
    int tileHeight{20};
    int rows{15};
    int columns{15};
    int tilesCount{15*15};
    TextureID id{};

    TextureData() = default;
    TextureData(const std::string& absolute_path, int imageWidth, int imageHeight, TextureID id, const std::string& name) : absolute_path(absolute_path), imageWidth(imageWidth), imageHeight(imageHeight), id(id), name(name) {}
    friend class LoaderBase;
};