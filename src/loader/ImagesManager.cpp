#include "ImagesManager.h"
#include "Settings.h"

TextureID ImagesManager::loadTextureToGRAMOrThrow(const ImageRaw& rawImageData, int tileWidth, int tileHeight, int rows, int columns, const std::string& name) {
    /* Store texture in GPU RAM */
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rawImageData.getWidth(), rawImageData.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rawImageData.getData().data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    bool textureWasNotCreated = (tid == 0);
    if(textureWasNotCreated) {
        throw std::runtime_error("Error storing texture to GPU RAM");
    }
 
    /* Map Texture ID to Texture Data*/
    TextureID textureID{tid};
    if (Settings::Debug::VERBOSE_IS_MORE) {
        std::cout << "Texture stored in GPU successfully with id: " << textureID << std::endl;
    }

    TextureData textureData{rawImageData.getPath(), rawImageData.getWidth(), rawImageData.getHeight(), textureID, name};
    textureData.tileWidth = tileWidth;
    textureData.tileHeight = tileHeight;
    textureData.rows = rows;
    textureData.columns = columns;
    textureData.tilesCount = rows * columns;

    texturesInGRAMRegister[textureID] = textureData;

    /* Assing Texture Data to Texture ID. Can be hazardous. */
    //todo remove
    textureID.assosiatedTextureData = &texturesInGRAMRegister[textureID];
    return textureID;
}


std::optional<TextureData> ImagesManager::getTextureDataByID(TextureID textureID) {
    auto it = texturesInGRAMRegister.find(textureID);
    if(it != texturesInGRAMRegister.end()) {
        return it->second;
    }

    return std::nullopt;
}