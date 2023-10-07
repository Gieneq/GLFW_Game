#pragma once
#include <string>
#include <vector>
#include <optional>
#include <map>

#include "TextureData.h"
#include "TextureId.h"
#include "LoaderDatatypes.h"

class ImagesManager {
public:
    ImagesManager() = default;
    ~ImagesManager() = default;

    TextureID loadTextureToGRAMOrThrow(const ImageRaw& rawImageData, int tileWidth, int tileHeight, int rows, int columns, const std::string& name);

    std::optional<TextureData> getTextureDataByID(TextureID textureID);

private:
    std::map<TextureID, TextureData> texturesInGRAMRegister;
};