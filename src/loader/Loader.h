#pragma once
#include <string>
#include <vector>
#include "lodepng.h"
#include "World.h"
#include "TextureData.h"
#include "TextureId.h"
#include <optional>
#include <map>
#include <tuple>
#include "pugixml.hpp"
#include "Maths.h"

/**
 * Group is Floor class - heigt of terrain
 * Layer is loading part of Group/Floor
*/

// #define BUILD_TESTWORLD 1
#define USE_ONLY_0_GROUP 1
// #define USE_ONLY_0_LAYER 1

class TileData {
public:
    TileData() = default;
    TileData(int tileLID) : tileLID(tileLID) {}
    int tileLID{-1};

    int animationInterval{1000};
    std::vector<int> animationFramesLIDs;
    std::vector<Rect2F> collisionRects;
    
    bool hasAnimation() const {
        return !animationFramesLIDs.empty();
    }

    bool hasCollisionRects() const {
        return !collisionRects.empty();
    }

    friend std::ostream& operator<<(std::ostream& os, const TileData& tileData) {
        os << "   - TileData[" << tileData.tileLID << "]" << ", AnimationsCount: " << tileData.animationFramesLIDs.size() << ", collisionRectsCount: " << tileData.collisionRects.size() << std::endl;
        
        if(tileData.hasAnimation()) {
            os << "     - AnimationFrames: ";
            for(const auto& animationFrameLID : tileData.animationFramesLIDs) {
                os << animationFrameLID << ", ";
            }
            os << std::endl;
        }

        if(tileData.hasCollisionRects()) {
            os << "     - CollisionRects: ";
            for(const auto& collisionRect : tileData.collisionRects) {
                os << collisionRect << ", ";
            }
            os << std::endl;
        }
        return os;
    }
};

class TilesetData {
public:
    TilesetData(int firstGID, std::optional<int> lastGID, const std::string& tilesetAbsolutePath, 
        const std::string& tilesetImageAbsolutePath, int imageWidth, int imageHeight, int columns, int rows, int tileWidth, int tileHeight) : firstGID(firstGID), lastGID(lastGID), tilesetAbsolutePath(tilesetAbsolutePath), 
        tilesetImageAbsolutePath(tilesetImageAbsolutePath), imageWidth(imageWidth), imageHeight(imageHeight), columns(columns), rows(rows), tileWidth(tileWidth), tileHeight(tileHeight) {}
    
    int firstGID;
    std::optional<int> lastGID;
    std::string tilesetAbsolutePath;
    std::string tilesetImageAbsolutePath;
    int imageWidth;
    int imageHeight;
    int columns;
    int rows;
    int tileWidth;
    int tileHeight;
    int tileWidthMultiplier{1};
    int tileHeightMultiplier{1};
    TextureID textureID;
    std::vector<TileData> tilesData;

    void setTextureID(TextureID textureID) {
        this->textureID = textureID;
    }

    std::optional<TileData> getTileDataByGID(int gid) const;

    std::optional<int> decodeGID(int gid) const;

    bool validateLID(int lid) const;

    friend std::ostream& operator<<(std::ostream& os, const TilesetData& tilesetData) {
        os << "   - TilesetData [" << tilesetData.firstGID << ", " << tilesetData.lastGID.value_or(-1) << "] " << std::endl;
        os << "   - tilesetAbsolutePath: " << tilesetData.tilesetAbsolutePath << std::endl;
        os << "   - tilesetImageAbsolutePath: " << tilesetData.tilesetImageAbsolutePath << std::endl;
        os << "   - imageWidth: " << tilesetData.imageWidth << std::endl;
        os << "   - imageHeight: " << tilesetData.imageHeight << std::endl;
        os << "   - columns: " << tilesetData.columns << std::endl;
        os << "   - rows: " << tilesetData.rows << std::endl;
        os << "   - textureID: " << tilesetData.textureID << std::endl;
        os << "   - tilesDataCount: " << tilesetData.tilesData.size() << ", details:" << std::endl;
        
        for(const auto& tileData : tilesetData.tilesData) {
            os << tileData;
        }

        return os;
    }
};

class MapData {
public:
    MapData() = default;
    MapData(const std::string& mapAbsolutePath, int width, int height, int tileWidth, int tileHeight) : mapAbsolutePath(mapAbsolutePath), width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight) {}
    std::string mapAbsolutePath{};
    int width{-1};
    int height{-1};
    int tileWidth{-1};
    int tileHeight{-1};
    std::vector<TilesetData> tilesetsData;

    void addTilesetData(const TilesetData& tilesetData) {
        tilesetsData.push_back(tilesetData);
    }

    std::optional<TilesetData> getTilesetDataCorrespondingToGID(int gid) const;

    friend std::ostream& operator<<(std::ostream& os, const MapData& mapData) {
        os << "MapData: " << std::endl;
        os << " - mapPath: " << mapData.mapAbsolutePath << std::endl;
        os << " - width: " << mapData.width << std::endl;
        os << " - height: " << mapData.height << std::endl;
        os << " - tileWidth: " << mapData.tileWidth << std::endl;
        os << " - tileHeight: " << mapData.tileHeight << std::endl;
        os << " - tilesetsData: " << std::endl;
        for(const auto& tilesetData : mapData.tilesetsData) {
            os << tilesetData << std::endl;
        }
        return os;
    }
};


class Loader {
private:
    Loader() = default;
    ~Loader() = default;
public:
    static Loader& getLoader() {
        static Loader loader;
        return loader;
    }

    bool loadAssets();
    bool loadWorld(World& world);
    bool loadPlayer(World& world);

    friend std::ostream& operator<<(std::ostream& os, const Loader& loader) {
        os << "Loader: " << loader.textureDatas.size() << " textures:" << std::endl;
        for(const auto& textureData : loader.textureDatas) {
            os << " - " << textureData << ", " << std::endl;
        }
        return os;
    }

    bool hasTextureDataWithID(TextureID textureID);
    std::optional<TextureData> getTextureDataByID(TextureID textureID);
    bool hasTextureDataWithName(const std::string& name);
    std::optional<TextureData> getTextureDataByName(const std::string& name);
    
protected:
    std::optional<TextureID> loadTextureFromAbsolutePath(const std::string& abs_path, int div_w, int div_h, const std::string& name);
    std::optional<TextureID> loadTextureFromAssets(const std::string& relativePath, int div_w, int div_h, const std::string& name);
    std::optional<MapData> loadMapData(World& world, const std::string& mapName);
    // std::optional<std::vector<Entity*>> Loader::createEntitiesLayer(const MapData& mapData, const std::vector<int> layerDataIndices, Floor* containingFloor);
    bool buildWorld(World& world, const std::string mapName, const MapData& mapData);
    bool fillElevationWithEntities(World& world, Elevation* elevation, EntityType entityType, const MapData& mapData, const std::vector<int> layerDataIndices);

private:
    std::optional<std::map<std::string, int>> getMapInfo(const pugi::xml_node& mapNode);
    std::vector<std::tuple<int, std::optional<int>, std::string>> getTilesetsInfo(const pugi::xml_node& mapNode);
    std::optional<TilesetData> loadTilesetData(int firstGid, std::optional<int> lastGid, const std::string& mapPath, const std::string& tilesetRelativePath, const int mapTileWidth, const int mapTileHeight);
    bool loadTilesData(const pugi::xml_node& mapNode, TilesetData& tilesetData);
    std::string getMapAbsolutePath(const std::string& mapName);
    std::string getTilesetAbsolutePath(const std::string& mapPath, const std::string& tilesetRelativePath);
    std::string getTilesetImageAbsolutePath(const std::string& tilesetAbsolutePath, const std::string& imageName);
    std::optional<TextureID> storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height);
    
    std::vector<TextureData> textureDatas{};
    MapData mapData{};
};