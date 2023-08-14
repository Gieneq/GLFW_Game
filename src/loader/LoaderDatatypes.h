#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <optional>
#include "Maths.h"
#include "TextureData.h"
#include "TextureId.h"

class TilesetData;
class MapData;
class Loader;

class TileData {
public:
    bool hasAnimation() const {
        return !animationFramesLIDs.empty();
    }

    bool hasCollisionRects() const {
        return !collisionRects.empty();
    }

    int getTileLID() const {
        return tileLID;
    }

    int getAnimationInterval() const {
        return animationInterval;
    }

    const std::vector<int>& getAnimationFramesLIDs() const {
        return animationFramesLIDs;
    }

    std::vector<TileData*> getAnimationFramesTileDatas();

    const std::vector<Rect2F>& getCollisionRects() const {
        return collisionRects;
    }

    TilesetData* getContainingTilesetData() const {
        return containingTilesetData;
    }

    friend std::ostream& operator<<(std::ostream& os, const TileData& tileData) {
        os << "[" << tileData.tileLID << "]" << ", AnimationsCount: " << tileData.animationFramesLIDs.size() << ", collisionRectsCount: " << tileData.collisionRects.size() << std::endl;
        return os;
    }

private:
    TileData() = default;
    TileData(int tileLID) : tileLID(tileLID) {}
    int tileLID{-1};
    TilesetData* containingTilesetData{nullptr};
    int animationInterval{1000};
    std::vector<int> animationFramesLIDs;
    std::vector<Rect2F> collisionRects;

    friend class TilesetData;
    friend class Loader;
};

class TilesetData {
public:
    TilesetData() = default;

    TilesetData(int firstGID, std::optional<int> lastGID) : firstGID(firstGID), lastGID(lastGID) {}
    
    inline int getColumnCount() const {
        return columns;
    }

    inline int getRowCount() const {
        return rows;
    }

    inline int getTileWidth() const {
        return tileWidth;
    }

    inline int getTileHeight() const {
        return tileHeight;
    }

    float getTileRelativeWidthtScale() const;

    float getTileRelativeHeightScale() const;

    TextureID getTextureID() {
        return textureID;
    }

    TileData* appendTileData(int lid);

    std::optional<TileData*> getTileDataByGID(int gid);
    TileData* TilesetData::getTileDataByLID(int lid);

    std::optional<int> decodeGID(int gid) const;

    bool validateLID(int lid) const;

    friend std::ostream& operator<<(std::ostream& os, const TilesetData& tilesetData) {
        os << "[" << tilesetData.firstGID << ", " << tilesetData.lastGID.value_or(-1) << "] " << std::endl;
        return os;
    }

private:
    int columns;
    int rows;
    int tileWidth;
    int tileHeight;

    MapData* containingMapData{nullptr};
    std::string tilesetAbsolutePath{};
    std::string tilesetImageAbsolutePath{};
    TextureID textureID{};
    int firstGID{-1};
    std::optional<int> lastGID{std::nullopt};

    std::vector<TileData> tileDatas;
    
    friend class Loader;
    friend class MapData;
};

class MapData {
public:

    inline int getTotalWidth() const {
        return widthTotal;
    }

    inline int getTotalHeight() const {
        return heightTotal;
    }

    inline int getBaseTileWidth() const {
        return tileWidthBase;
    }

    inline int getBaseTileHeight() const {
        return tileHeightBase;
    }

    std::optional<TilesetData*> getTilesetDataCorrespondingToGID(int gid);

    std::optional<TileData*> getTileDataCorrespondingToGID(int gid);

    friend std::ostream& operator<<(std::ostream& os, const MapData& mapData) {
        os << "MapData: " << std::endl;
        os << " - mapPath: " << mapData.mapAbsolutePath << std::endl;
        os << " - width: " << mapData.widthTotal << std::endl;
        os << " - height: " << mapData.heightTotal << std::endl;
        os << " - tileWidth: " << mapData.tileWidthBase << std::endl;
        os << " - tileHeight: " << mapData.tileHeightBase << std::endl;
        os << " - tilesetsData: " << mapData.tilesetDatas.size() << ", details:" << std::endl;
        for(const auto& tilesetData : mapData.tilesetDatas) {
            os << tilesetData << std::endl;
        }
        return os;
    }

private:
    MapData() = default;

    bool create(const std::string& mapAbsolutePath, int width, int height, int tileWidth, int tileHeight);

    bool validate() const;

    TilesetData* appendTilesetData(int firstGID, std::optional<int> lastGID);

    std::string mapAbsolutePath{};
    int widthTotal{-1};
    int heightTotal{-1};
    int tileWidthBase{-1};
    int tileHeightBase{-1};
    std::vector<TilesetData> tilesetDatas;

    friend class Loader;
};