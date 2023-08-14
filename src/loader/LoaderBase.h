#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <optional>

#include "TextureData.h"
#include "TextureId.h"

class LoaderBase {
protected:
    LoaderBase() = default;
    ~LoaderBase() = default;
public:
    static LoaderBase& getInstance() {
        static LoaderBase loader;
        return loader;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const LoaderBase& loader) {
        os << "LoaderBase: " << loader.textureDatas.size() << " textures:" << std::endl;
        for(const auto& textureData : loader.textureDatas) {
            os << " - " << textureData << ", " << std::endl;
        }
        return os;
    }

    bool hasTextureDataWithID(TextureID textureID); 
    
    std::optional<TextureData*> getTextureDataByID(TextureID textureID);
    
    bool hasTextureDataWithName(const std::string& name);
    
    std::optional<TextureData*> getTextureDataByName(const std::string& name);
    
protected:
    std::optional<TextureID> loadTextureFromAbsolutePath(const std::string& abs_path, int tileWidth, int tileHeight, int rows, int columns, const std::string& name); 
    
    std::optional<TextureID> loadTextureFromAssets(const std::string& relativePath, int tileWidth, int tileHeight, int rows, int columns, const std::string& name); 
    
    
    std::string getMapAbsolutePath(const std::string& mapName);

    std::string getTilesetAbsolutePath(const std::string& mapPath, const std::string& tilesetRelativePath);

    std::string getTilesetImageAbsolutePath(const std::string& tilesetAbsolutePath, const std::string& imageName);

private:    
    std::optional<TextureID> storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height); 
    
    std::vector<TextureData> textureDatas{};
};