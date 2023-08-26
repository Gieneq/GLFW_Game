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
        return !collisionCuboids.empty();
    }

    int getTileLID() const {
        return tileLID;
    }

    int getAnimationInterval() const {
        return animationInterval;
    }

    inline int getAnimationFramesCount() const {
        return static_cast<int>(animationFramesLIDs.size());
    }

    inline int getCollisionRectsCount() const {
        return static_cast<int>(collisionCuboids.size());
    }

    std::vector<int>::iterator animationFramesBegin() {
        return animationFramesLIDs.begin();
    }

    std::vector<int>::iterator animationFramesEnd() {
        return animationFramesLIDs.end();
    }

    std::vector<int>::const_iterator animationFramesBegin() const {
        return animationFramesLIDs.begin();
    }

    std::vector<int>::const_iterator animationFramesEnd() const {
        return animationFramesLIDs.end();
    }

    std::vector<Cuboid6F>::iterator collisionRectsBegin() {
        return collisionCuboids.begin();
    }

    std::vector<Cuboid6F>::iterator collisionRectsEnd() {
        return collisionCuboids.end();
    }

    std::vector<Cuboid6F>::const_iterator collisionRectsBegin() const {
        return collisionCuboids.begin();
    }

    std::vector<Cuboid6F>::const_iterator collisionRectsEnd() const {
        return collisionCuboids.end();
    }

    std::vector<TileData*> getAnimationFramesTileDatas();

    TilesetData* getContainingTilesetData() const {
        return containingTilesetData;
    }

    friend std::ostream& operator<<(std::ostream& os, const TileData& tileData) {
        os << "[" << tileData.tileLID << "]" << ", AnimationsCount: " << tileData.getAnimationFramesCount() << ", collisionRectsCount: " << tileData.getCollisionRectsCount() << std::endl;
        return os;
    }

private:
    TileData() = default;
    TileData(int tileLID) : tileLID(tileLID) {}
    int tileLID{-1};
    TilesetData* containingTilesetData{nullptr};
    int animationInterval{1000};
    std::vector<int> animationFramesLIDs;
    std::vector<Cuboid6F> collisionCuboids;
    bool walkable{false};
    // int depthPx{20};

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

    inline int getTilesDataCount() const {
        return static_cast<int>(tileDatas.size());
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

    inline int getTilesetsCount() const {
        return static_cast<int>(tilesetDatas.size());
    }

    inline int getAllTileDatasCount() const {
        int count = 0;
        for(const auto& tilesetData : tilesetDatas) {
            count += tilesetData.getTilesDataCount();
        }
        return count;
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