#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "lodepng.h"
#include "World.h"
#include "TilesetData.h"
#include "TileId.h"
#include "TextureData.h"
#include <optional>

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
    TextureId load_image(std::string abs_path);
    TextureId loadTextureFromAssets(std::string res_path);

    bool hasTextureDataWithID(TextureId textureID) {
        return textureDatas.find(textureID) != textureDatas.end();
    }
    std::optional<TextureData> getTextureDataByID(TextureId key_texture_id);

    bool hasTextureIDWithName(const std::string& name) {
        return texturesIDsRegister.find(name) != texturesIDsRegister.end();
    }
    std::optional<TextureId> getTextureIDByName(const std::string& name);

    void __load_map(World& world, std::string map_name);

private:
    bool registerTextureStringName(const std::string& name, const TextureId& id);
    TextureData storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height, std::string abs_path);

    /**
     * IDs for textures.
     * 
     * Texture IDs are used to identify textures in the GPU.
     * They are mapped to string keys.
     * 
     * TextureData and TilesetData have additional information, e.g. width and height.
    */
    std::unordered_map<TextureId, TextureData, TextureId> textureDatas{};
    std::unordered_map<std::string, TextureId> texturesIDsRegister{};
    std::unordered_map<TextureId, TilesetData, TextureId> tilesetDatas{};

};