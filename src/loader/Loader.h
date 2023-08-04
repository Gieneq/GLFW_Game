#pragma once
#include <string>
#include <vector>
#include "lodepng.h"
#include "World.h"
#include "TextureData.h"
#include "TextureId.h"
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
    bool Loader::loadTextureFromAbsolutePath(const std::string& abs_path, int div_w, int div_h, const std::string& name);
    bool Loader::loadTextureFromAssets(const std::string& relativePath, int div_w, int div_h, const std::string& name);

private:
    std::optional<TextureID> Loader::storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height);
    std::vector<TextureData> textureDatas{};
};