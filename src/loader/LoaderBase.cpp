#include "LoaderBase.h"

#include "Settings.h"
#include "lodepng.h"
#include "IOPath.h"
#include <fstream>
#include <windows.h>

//add unique pointer
#include <memory>

#define CPPPATH_SEP "/"
#include <cpppath.h>

bool LoaderBase::init() {
    bool result = true;

    const auto worldFilePath = cpppath::join({Settings::Resources::RES_ROOT_DIRPATH, 
        Settings::Resources::WORLD_DIRPATH,
        Settings::Resources::WORLD_FILENAME});

    std::cout << "worldFilePath: " << worldFilePath << std::endl;

    auto textContent = getFileTextOrThrow(worldFilePath);

    auto jsonWrapper = getFileJSONOrThrow(worldFilePath);

    const auto jsonWrapperText = cJSON_Print(jsonWrapper.value());

    std::cout << "jsonWrapper: " << jsonWrapperText << std::endl;

    // throw std::runtime_error("LoaderBase::init() not implemented");
    return result;
}


std::string LoaderBase::getFileTextOrThrow(const std::string& absolutePath) {
    if (cpppath::exists(absolutePath) == false) {
        throw std::runtime_error("LoaderBase::getFileTextOrThrow() - file does not exist: " + absolutePath);
    }

    std::ifstream ifs(absolutePath);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}


CJSONWrapper LoaderBase::getFileJSONOrThrow(const std::string& absolutePath) {
    const auto fileText = getFileTextOrThrow(absolutePath);
    auto json = cJSON_Parse(fileText.c_str());
    if (json == nullptr) {
        throw std::runtime_error("LoaderBase::getFileJSONOrThrow() - error parsing json: " + std::string(cJSON_GetErrorPtr()));
    }

    return CJSONWrapper(json);
}


ImageRaw LoaderBase::getPNGOrThrow(const std::string& absolutePath) {
    if (cpppath::exists(absolutePath) == false) {
        throw std::runtime_error("LoaderBase::getFileTextOrThrow() - file does not exist: " + absolutePath);
    }

    std::vector<unsigned char> image;
    unsigned imageWidth, imageHeight;
    unsigned loadingDecodingError = lodepng::decode(image, imageWidth, imageHeight, absolutePath.c_str());
    bool hasError = loadingDecodingError != 0;
    if(hasError) {
        throw std::runtime_error("LoaderBase::getPNGOrThrow() - error loading png: " + std::string(lodepng_error_text(loadingDecodingError)));
    }

    return ImageRaw(imageWidth, imageHeight, std::move(image), absolutePath);
}








bool LoaderBase::hasTextureDataWithID(TextureID textureID) {
    return getTextureDataByID(textureID).has_value();
}


std::optional<TextureData*> LoaderBase::getTextureDataByID(TextureID textureID) {
    auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [textureID](const TextureData& textureData) {
        return textureData.id == textureID;
    });

    if(it != textureDatas.end()) {
        return &(*it);
    }

    return std::nullopt;
}

bool LoaderBase::hasTextureDataWithName(const std::string& name) {
    return getTextureDataByName(name).has_value();
}

std::optional<TextureData*> LoaderBase::getTextureDataByName(const std::string& name) {
    auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [name](const TextureData& textureData) {
        // std::cout << "comparing: " << textureData.name << " with " << name << std::endl;
        return textureData.name == name;
    });

    if(it != textureDatas.end()) {
        return &(*it);
    }

    return std::nullopt;
}

std::optional<TextureID> LoaderBase::storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height) {
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    bool textureWasNotCreated = tid == 0;
    if(textureWasNotCreated) {
        std::cerr << "Error creating texture in GPU" << std::endl;
        return std::nullopt;
    }
 
    TextureID textureID{tid};
    std::cout << "Texture stored in GPU successfully with id: " << textureID << std::endl;
    return textureID;
}

std::optional<TextureID> LoaderBase::loadTextureFromAssets(const std::string& relativePath, int tileWidth, int tileHeight, int rows, int columns, const std::string& name) {
    std::string absPath = IOPath::get_absolute_path(Settings::Resources::ASSETS_PATH + relativePath);
    return loadTextureFromAbsolutePath(absPath, tileWidth, tileHeight, rows, columns, name);
}

std::optional<TextureID> LoaderBase::loadTextureFromAbsolutePath(const std::string& abs_path, int tileWidth, int tileHeight, int rows, int columns, const std::string& name) {
    /* Check if image with given path is already loaded. */
    const std::string absolute_path = IOPath::get_absolute_path(abs_path);
    for(const auto& textureData : textureDatas) {
        if(textureData.absolute_path == absolute_path) {
            /* It is considered as error */
            std::cerr << "Image with path: " << absolute_path << " is already loaded." << std::endl;
            return std::nullopt;
        }
    }

    /* Check if image with given name is already loaded. */
    if(!name.empty()) {
        auto textureDataOption = getTextureDataByName(name);
        /* Check if found any */
        if(textureDataOption.has_value()) {
            /* It is considered as error */
            std::cerr << "Image with name: " << name << " is already loaded." << std::endl;
            return std::nullopt;
        }
    }

    /* Load and decode file to pixels data */
    std::vector<unsigned char> image;
    unsigned imageWidth, imageHeight;
    unsigned loadingDecodingError = lodepng::decode(image, imageWidth, imageHeight, absolute_path.c_str());
    bool hasError = loadingDecodingError != 0;

    if(hasError) {
        std::cerr << "Error during loading file " << absolute_path << " " << loadingDecodingError << ": " << lodepng_error_text(loadingDecodingError) << std::endl;
        return std::nullopt;
    }

    /* Seem this texture was not loaded yet. Store it in GPU memory. */
    auto textureIDOption = storeInGPUMemory(image, imageWidth, imageHeight);
    if(!textureIDOption.has_value()) {
        std::cerr << "Error storing image " << absolute_path << " in GPU memory" << std::endl;
        return std::nullopt;
    }
    auto textureID = textureIDOption.value();

    /* Data successfully stored */
    image.clear();

    /* Check name */
    std::string textureName = "Texture_" + std::to_string((textureID).value);
    if(!name.empty()) {
        textureName = name;
    }

    int imageWidthInt = static_cast<int>(imageWidth);
    int imageHeightInt = static_cast<int>(imageHeight);

    /* Check sizes from arguments */
    if((imageWidthInt < tileWidth * columns) || (imageHeightInt < tileHeight * rows)) {
        std::cerr << "Error loading texture " << absolute_path << " - image size is smaller than given tile sizes" << std::endl;
        return std::nullopt;
    }

    /* Build final texture data */
    TextureData textureData (
        absolute_path, 
        imageWidthInt, 
        imageHeightInt, 
        textureID, 
        textureName
    );
    textureData.rows = rows;
    textureData.columns = columns;
    textureData.tileWidth = tileWidth;
    textureData.tileHeight = tileHeight;
    textureData.tilesCount = rows * columns;

    /* Store and assign data pointer */
    textureDatas.push_back(textureData);
    textureID.assosiatedTextureData = &textureDatas.back();

    std::cout << "Texture loaded successfully: " << textureData.getTextureID() << std::endl;
    return textureData.getTextureID();
}

std::string LoaderBase::getTilesetAbsolutePath(const std::string& mapPath, const std::string& tilesetRelativePath) {
    /* TilesetRelativePath is in relation to map containing this dir */
    std::string mapDir = IOPath::get_containing_dir(mapPath);
    std::string absoluteTilesetPath = IOPath::get_absolute_path(mapDir + "\\" + tilesetRelativePath);
    return absoluteTilesetPath;
}

std::string LoaderBase::getTilesetImageAbsolutePath(const std::string& tilesetAbsolutePath, const std::string& imageName) {
    /* absoluteTilesetImagePath is in the same dir as  tilesetAbsolutePath */
    std::string tilesetDir = IOPath::get_containing_dir(tilesetAbsolutePath);
    std::string absoluteTilesetImagePath = IOPath::get_absolute_path(tilesetDir + "\\" + imageName);
    return absoluteTilesetImagePath;
}

std::string LoaderBase::getMapAbsolutePath(const std::string& mapName) {
    std::string mapDir = std::string(Settings::Resources::DATA_PATH)
        + std::string(Settings::Resources::MAPS_DIR);
    std::string absoluteMapPath = IOPath::get_absolute_path(mapDir + mapName);
    return absoluteMapPath;
}
