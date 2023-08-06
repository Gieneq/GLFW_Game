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

// #define BUILD_TESTWORLD 1

class TilesetData {
public:
    TilesetData(int firstGID, std::optional<int> lastGID, const std::string& tilesetAbsolutePath, 
        const std::string& tilesetImageAbsolutePath, int imageWidth, int imageHeight, int columns, int rows) : firstGID(firstGID), lastGID(lastGID), tilesetAbsolutePath(tilesetAbsolutePath), 
        tilesetImageAbsolutePath(tilesetImageAbsolutePath), imageWidth(imageWidth), imageHeight(imageHeight), columns(columns), rows(rows) {}
    
    int firstGID;
    std::optional<int> lastGID;
    std::string tilesetAbsolutePath;
    std::string tilesetImageAbsolutePath;
    int imageWidth;
    int imageHeight;
    int columns;
    int rows;
    TextureID textureID;
    //todo vector if tilesdata

    void setTextureID(TextureID textureID) {
        this->textureID = textureID;
    }

    int getTileWidth() const {
        return imageWidth / columns;
    }

    int getTileHeight() const {
        return imageHeight / rows;
    }

    std::optional<int> getTileLID(int gid) const {
        if(gid < firstGID) {
            return std::nullopt;
        }
        if(lastGID.has_value() && gid > lastGID.value()) {
            return std::nullopt;
        }
        return gid - firstGID;
    }

    bool isGidInTileset(int gid) const {
        if(gid < firstGID) {
            return false;
        }
        if(lastGID.has_value() && gid > lastGID.value()) {
            return false;
        }
        return true; //todo consider checking if there is Tile Data
    }

    friend std::ostream& operator<<(std::ostream& os, const TilesetData& tilesetData) {
        os << "   - TilesetData [" << tilesetData.firstGID << ", " << tilesetData.lastGID.value_or(-1) << "] " << std::endl;
        os << "   - tilesetAbsolutePath: " << tilesetData.tilesetAbsolutePath << std::endl;
        os << "   - tilesetImageAbsolutePath: " << tilesetData.tilesetImageAbsolutePath << std::endl;
        os << "   - imageWidth: " << tilesetData.imageWidth << std::endl;
        os << "   - imageHeight: " << tilesetData.imageHeight << std::endl;
        os << "   - columns: " << tilesetData.columns << std::endl;
        os << "   - rows: " << tilesetData.rows << std::endl;
        os << "   - textureID: " << tilesetData.textureID << std::endl;
        return os;
    }
};

class MapData {
public:
    MapData(const std::string& mapAbsolutePath, int width, int height, int tileWidth, int tileHeight) : mapAbsolutePath(mapAbsolutePath), width(width), height(height), tileWidth(tileWidth), tileHeight(tileHeight) {}
    std::string mapAbsolutePath;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<TilesetData> tilesetsData;

    void addTilesetData(const TilesetData& tilesetData) {
        tilesetsData.push_back(tilesetData);
    }

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

private:
    std::optional<std::map<std::string, int>> getMapInfo(const pugi::xml_node& mapNode);
    std::vector<std::tuple<int, std::optional<int>, std::string>> getTilesetsInfo(const pugi::xml_node& mapNode);
    std::optional<TilesetData> loadTilesetData(int firstGid, std::optional<int> lastGid, const std::string& mapPath, const std::string& tilesetRelativePath);
    std::string getMapAbsolutePath(const std::string& mapName);
    std::string getTilesetAbsolutePath(const std::string& mapPath, const std::string& tilesetRelativePath);
    std::string getTilesetImageAbsolutePath(const std::string& tilesetAbsolutePath, const std::string& imageName);
    std::optional<TextureID> storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height);
    std::vector<TextureData> textureDatas{};
};