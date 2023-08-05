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
    bool loadTextureFromAbsolutePath(const std::string& abs_path, int div_w, int div_h, const std::string& name);
    bool loadTextureFromAssets(const std::string& relativePath, int div_w, int div_h, const std::string& name);
    bool loadMap(World& world, const std::string& mapName);

private:
    std::optional<std::map<std::string, int>> getMapInfo(const pugi::xml_node& mapNode);
    std::vector<std::tuple<int, std::optional<int>, std::string>> getTilesetsInfo(const pugi::xml_node& mapNode);
    bool loadTilesetData(int firstGid, std::optional<int> lastGid, const std::string& tilesetRelativePath);
    std::string getMapAbsolutePath(const std::string& mapName);
    std::optional<TextureID> storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height);
    std::vector<TextureData> textureDatas{};
};