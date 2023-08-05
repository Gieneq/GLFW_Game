#include "Loader.h"

#include <iostream>
#include "Settings.h"
#include "lodepng.h"
#include "IO.h"
#include "LocationComponent.h"
#include "GraphicsComponent.h"
#include "MovementComponent.h"
#include "ControllableComponent.h"
#include <algorithm>

bool Loader::loadAssets() {
    std::cout << "Loading assets data..." << std::endl;
    
    /* Load test tiles texture */
    auto loadingResult = Loader::getLoader().loadTextureFromAssets("tiles.png", 64, 64, "some_tiles");
    if(!loadingResult) {
        std::cerr << "Error loading texture from assets" << std::endl;
        return false;
    }

    std::cout << Loader::getLoader() << std::endl;
    return true;
}

bool Loader::loadWorld(World& world) {
    std::cout << "Loading world data..." << std::endl;

#ifdef BUILD_TESTWORLD
    /* Build test world */
    int size = 10;
    for(int y = -size; y < size+1; y++) {
        for(int x = -size; x < size+1; x++) {
            Entity *someTile = new Entity();

            auto locationCmp = new LocationComponent(someTile);
            locationCmp->worldRect.top_left.x = static_cast<float>(x);
            locationCmp->worldRect.top_left.y = static_cast<float>(y);
            locationCmp->worldRect.size.w = 1;
            locationCmp->worldRect.size.h = 1;
            someTile->addComponent(locationCmp);

            auto colorCmp = new ColorComponent(someTile, locationCmp);
            colorCmp->r = 0.5F + (y % 2) / 2.0F;
            colorCmp->g = 0.5F + (x % 2) / 2.0F;
            
            someTile->addComponent(colorCmp);
            world.entities.push_back(someTile);
        }
    }
#else
    /* Load world from files */
    loadMap(world, "testmap.tmx");
#endif

    return true;
}

bool Loader::loadPlayer(World& world) {
    std::cout << "Loading player data..." << std::endl;
    Entity* player = &world.player;

    /* Add player location component */
    auto locatiomCmp = new LocationComponent(player);
    player->addComponent(locatiomCmp);

    /* Set starting position */
    locatiomCmp->worldRect.top_left.x = 1.2F;
    locatiomCmp->worldRect.top_left.y = 0.0F;

    /* Try adding texture to player */
    auto playersTextureID = Loader::getLoader().getTextureDataByName("some_tiles");

    if(playersTextureID) {
        /* Texture */
        std::cout << "Player has texture" << std::endl;
        auto texture = new TextureComponent(player, locatiomCmp, (*playersTextureID).id);
        player->addComponent(texture);
    } 
    else {
        /* Color */
        std::cout << "Player has no texture, add placeholder color" << std::endl;
        auto color = new ColorComponent(player, locatiomCmp);
        color->r = 1.0F;
        color->g = 0.0F;
        color->b = 1.0F;
        player->addComponent(color);
    }

    /* Movement option */
    auto movementCmp = new MovementComponent(player, locatiomCmp);
    player->addComponent(movementCmp);
    movementCmp->setDirection(Direction::NONE);

    /* WSAD to control player */
    auto controllableCmp = new ControllableComponent(player, movementCmp);
    player->addComponent(controllableCmp);

    /* Yes, player is one of entities - 
     * easier to sort in rendering and similar. */
    world.entities.push_back(player);

    return true;
}

bool Loader::hasTextureDataWithID(TextureID textureID) {
    return getTextureDataByID(textureID).has_value();
}

std::optional<TextureData> Loader::getTextureDataByID(TextureID textureID) {
    auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [textureID](const TextureData& textureData) {
        return textureData.id == textureID;
    });

    if(it != textureDatas.end()) {
        return *it;
    }

    return std::nullopt;
}

bool Loader::hasTextureDataWithName(const std::string& name) {
    return getTextureDataByName(name).has_value();
}

std::optional<TextureData> Loader::getTextureDataByName(const std::string& name) {
    auto it = std::find_if(textureDatas.begin(), textureDatas.end(), [name](const TextureData& textureData) {
        std::cout << "comparing: " << textureData.name << " with " << name << std::endl;
        return textureData.name == name;
    });

    if(it != textureDatas.end()) {
        return *it;
    }

    return std::nullopt;
}

std::optional<TextureID> Loader::storeInGPUMemory(std::vector<unsigned char>& pixels, int width, int height) {
    GLuint tid;
    glGenTextures(1, &tid);
    glBindTexture(GL_TEXTURE_2D, tid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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

bool Loader::loadTextureFromAssets(const std::string& relativePath, int div_w, int div_h, const std::string& name) {
    std::string absPath = IO::get_absolute_path(Settings::Resources::ASSETS_PATH + relativePath);
    return loadTextureFromAbsolutePath(absPath, div_w, div_h, name);
}

bool Loader::loadTextureFromAbsolutePath(const std::string& abs_path, int div_w, int div_h, const std::string& name) {
    /* Check if image with given path is already loaded. */
    const std::string absolute_path = IO::get_absolute_path(abs_path);
    for(const auto& textureData : textureDatas) {
        if(textureData.absolute_path == absolute_path) {
            /* It is considered as error */
            std::cerr << "Image with path: " << absolute_path << " is already loaded." << std::endl;
            return false;
        }
    }

    /* Load and decode file to pixels data */
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned loadingDecodingError = lodepng::decode(image, width, height, absolute_path.c_str());
    bool hasError = loadingDecodingError != 0;

    if(hasError) {
        std::cerr << "Error during loading file " << absolute_path << " " << loadingDecodingError << ": " << lodepng_error_text(loadingDecodingError) << std::endl;
        return false;
    }

    /* Seem this texture was not loaded yet. Store it in GPU memory. */
    auto textureID = storeInGPUMemory(image, width, height);
    if(!textureID) {
        std::cerr << "Error storing image " << absolute_path << " in GPU memory" << std::endl;
        return false;
    }

    /* Data successfully stored */
    image.clear();

    /* Build final texture data */
    //const GLuint gl_id
    TextureData texture_data(absolute_path, width, height, *textureID, name);
    texture_data.div_width = div_w;
    texture_data.div_height = div_h;

    /* Store in set */
    textureDatas.push_back(texture_data);

    std::cout << "Texture loaded successfully: " << texture_data.id << std::endl;
    return true;
}

bool Loader::loadMap(World& world, const std::string& mapName) {
    /* Get absolute path to the map */
    auto mapAbsolutePath = getMapAbsolutePath(mapName);

    /* Load map file with pugixml */

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(mapAbsolutePath.c_str());
    if(!result) {
        std::cerr << "Error loading map file: " << result.description() << std::endl;
        return false;
    }

    /* Get map node and validate */
    auto mapNode = doc.child("map");
    if(!mapNode) {
        std::cerr << "Error loading map file: no map node in map file" << std::endl;
        return false;
    }

    /* Extract map info */
    auto mapInfoOption = getMapInfo(mapNode);
    if(!mapInfoOption.has_value()) {
        std::cerr << "Error loading map file: invalid map info" << std::endl;
        return false;
    }
    auto mapInfo = mapInfoOption.value();

    std::cout << "Map info: " << std::endl;
    std::cout << " - mapWidth: " << mapInfo["mapWidth"] << std::endl;
    std::cout << " - mapHeight: " << mapInfo["mapHeight"] << std::endl;
    std::cout << " - tileWidth: " << mapInfo["tileWidth"] << std::endl;
    std::cout << " - tileHeight: " << mapInfo["tileHeight"] << std::endl;

    /**
     * Extract tileset info - global tiles ids with their source file relative path.
     */
    auto tilesetsInfo = getTilesetsInfo(mapNode);
    if(tilesetsInfo.empty()) {
        std::cerr << "Error loading map file: invalid tilesets info" << std::endl;
        return false;
    }

    std::cout << "Tilesets info: " << std::endl;
    for(const auto& tilesetInfo : tilesetsInfo) {
        std::cout << " - firstGid: " << std::get<0>(tilesetInfo);
        std::cout << ", lastGid: " << std::get<1>(tilesetInfo).value_or(-1);
        std::cout << ", source: " << std::get<2>(tilesetInfo) << std::endl;
    }

    /* Load each tileset data */
    for(const auto& tilesetInfo : tilesetsInfo) {
        auto firstGid = std::get<0>(tilesetInfo);
        auto lastGid = std::get<1>(tilesetInfo);
        auto tilesetRelativePath = std::get<2>(tilesetInfo);

        auto tilesetData = loadTilesetData(firstGid, lastGid, tilesetRelativePath);
    }


    std::cout << "Map file loaded successfully" << std::endl;
    return true;
}

std::optional<std::map<std::string, int>> Loader::getMapInfo(const pugi::xml_node& mapNode) {
    /* Get all attributes heck if they exist */
    auto mapWidthAttrib = mapNode.attribute("width");
    auto mapHeightAttrib = mapNode.attribute("height");
    auto tileWidthAttrib = mapNode.attribute("tilewidth");
    auto tileHeightAttrib = mapNode.attribute("tileheight");

    /* Check if map has required attibutes */
    if(!mapWidthAttrib || !mapHeightAttrib || !tileWidthAttrib || !tileHeightAttrib) {
        std::cerr << "Error loading map file: no map size or tile size" << std::endl;
        return std::nullopt;
    }

    /* Get attibutes values */
    int mapWidth = mapNode.attribute("width").as_int(-1);
    int mapHeight = mapNode.attribute("height").as_int(-1);
    int tileWidth = mapNode.attribute("tilewidth").as_int(-1);
    int tileHeight = mapNode.attribute("tileheight").as_int(-1);

    /* Validate retrived values */
    if(mapWidth <= 0 || mapHeight <= 0 || tileWidth <= 0 || tileHeight <= 0) {
        std::cerr << "Error loading map file: invalid map size or tile size" << std::endl;
        return std::nullopt;
    }

    /* Build result */
    std::map<std::string, int> mapInfo;
    mapInfo["mapWidth"] = mapWidth;
    mapInfo["mapHeight"] = mapHeight;
    mapInfo["tileWidth"] = tileWidth;
    mapInfo["tileHeight"] = tileHeight;

    return mapInfo;
}


std::vector<std::tuple<int, std::optional<int>, std::string>> Loader::getTilesetsInfo(const pugi::xml_node& mapNode) {
    std::vector<std::tuple<int, std::optional<int>, std::string>> tilesetsInfo;

    /* Iterate over all tilesets */
    for(auto tilesetNode : mapNode.children("tileset")) {
        /* Get tileset attributes */
        auto firstGidAttrib = tilesetNode.attribute("firstgid");
        auto sourceAttrib = tilesetNode.attribute("source");

        /* Check if tileset has required attibutes */
        if(!firstGidAttrib || !sourceAttrib) {
            std::cerr << "Error loading map file: no tileset firstgid or source" << std::endl;
            return std::vector<std::tuple<int, std::optional<int>, std::string>>();
        }

        /* Get attibutes values */
        int firstGid = tilesetNode.attribute("firstgid").as_int(-1);
        std::string source = tilesetNode.attribute("source").as_string("");

        /* Validate retrived values */
        if(firstGid <= 0 || source.empty()) {
            std::cerr << "Error loading map file: invalid tileset firstgid or source" << std::endl;
            return std::vector<std::tuple<int, std::optional<int>, std::string>>();
        }

        /**
         * Update last tileset data with this firstGid.
         * Note: tilesets last Gid is next tilesets firstGid - 1.
         */
        if(!tilesetsInfo.empty()) {
            auto& lastTilesetInfo = tilesetsInfo.back();
            std::get<1>(lastTilesetInfo) = std::make_optional(firstGid - 1);
        }

        /* Build result */
        tilesetsInfo.push_back(std::make_tuple(firstGid, std::nullopt, source));
    }

    return tilesetsInfo;
}


/**
 * Load tileset data from file. Append to Loader's data. If return true - success.
*/
bool Loader::loadTilesetData(int firstGid, std::optional<int> lastGid, const std::string& tilesetRelativePath) {
    return true;
}

std::string Loader::getMapAbsolutePath(const std::string& mapName) {
    std::string mapDir = std::string(Settings::Resources::DATA_PATH)
        + std::string(Settings::Resources::MAPS_DIR);
    std::string absoluteMapPath = IO::get_absolute_path(mapDir + mapName);
    // std::cout << "Map absolute path: " << absoluteMapPath << std::endl;

    return absoluteMapPath;
}







