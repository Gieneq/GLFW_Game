#include "Loader.h"

#include <iostream>
#include "Settings.h"
#include "lodepng.h"
#include "IO.h"
#include "LocationComponent.h"
#include "GraphicsComponent.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "ControllableComponent.h"
#include "CollisionComponents.h"
#include <algorithm>
#include <sstream>
#include "Timers.h"


std::optional<TileData> TilesetData::getTileDataByGID(int gid) const {
    /* Check if this tileset can have corresponding GID */
    auto lidOption = decodeGID(gid);
    if(!lidOption.has_value()) {
        return std::nullopt;
    }

    auto lid = lidOption.value();

    /* Has LID, find TileData in vector */
    for(const auto& tileData : tilesData) {
        if(tileData.tileLID == lid) {
            return tileData;
        }
    }

    /* No tile data found - generate with lid */
    TileData tileData{lid};
    //todo append
    return tileData;
}

std::optional<int> TilesetData::decodeGID(int gid) const {
    if(gid < firstGID) {
        return std::nullopt;
    }
    if(lastGID.has_value() && gid > lastGID.value()) {
        return std::nullopt;
    }
    return gid - firstGID;
}

bool TilesetData::validateLID(int lid) const {
    if(lid < 0) {
        return false;
    }
    if(lid >= columns * rows) {
        return false;
    }
    return true;
}

std::optional<TilesetData> MapData::getTilesetDataCorrespondingToGID(int gid) const {
    for(const auto& tilesetData : tilesetsData) {
        auto tileDataOption = tilesetData.getTileDataByGID(gid);
        if(tileDataOption.has_value()) {
            return tilesetData;
        }
    }
    return std::nullopt;
}

/* Loader */

bool Loader::loadAssets() {
    std::cout << "Loading assets data..." << std::endl;
    
    /* Load test tiles texture */
    auto loadingResult = Loader::getLoader().loadTextureFromAssets("tiles.png", 64, 64, "some_tiles");
    if(!loadingResult) {
        std::cerr << "Error loading texture from assets" << std::endl;
        return false;
    }

    // std::cout << Loader::getLoader() << std::endl;
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
    /**
     * Load map data from files.
     * Tiles are not built yet.
     * It is used to build tiles and populate world.
     */
    const std::string mapFileName = "testmap.tmx";
    {
        auto mapDataOption = loadMapData(world, mapFileName);
        if(!mapDataOption.has_value()) {
            std::cerr << "Error loading map data" << std::endl;
            return false;
        }

        mapData = mapDataOption.value();
        /* Destroy on out of scope */
    }

    /* Use mapData to build World */
    auto worldBuildResult = buildWorld(world, mapFileName, mapData);

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
    locatiomCmp->worldRect.top_left.x = 20.0F;
    locatiomCmp->worldRect.top_left.y = 30.0F;

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
    movementCmp->speed = Settings::Player::INITIAL_SPEED;
    player->addComponent(movementCmp);
    movementCmp->setDirection(Direction::NONE);

    /* WSAD to control player */
    auto controllableCmp = new ControllableComponent(player, movementCmp);
    player->addComponent(controllableCmp);

    /* Collision detector */
    auto collisionDetectorCmp = new CollisionDetectorComponent(player, movementCmp);
    player->addComponent(collisionDetectorCmp);

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
        // std::cout << "comparing: " << textureData.name << " with " << name << std::endl;
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

std::optional<TextureID> Loader::loadTextureFromAssets(const std::string& relativePath, int div_w, int div_h, const std::string& name) {
    std::string absPath = IO::get_absolute_path(Settings::Resources::ASSETS_PATH + relativePath);
    return loadTextureFromAbsolutePath(absPath, div_w, div_h, name);
}

std::optional<TextureID> Loader::loadTextureFromAbsolutePath(const std::string& abs_path, int div_w, int div_h, const std::string& name) {
    /* Check if image with given path is already loaded. */
    const std::string absolute_path = IO::get_absolute_path(abs_path);
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
    unsigned width, height;
    unsigned loadingDecodingError = lodepng::decode(image, width, height, absolute_path.c_str());
    bool hasError = loadingDecodingError != 0;

    if(hasError) {
        std::cerr << "Error during loading file " << absolute_path << " " << loadingDecodingError << ": " << lodepng_error_text(loadingDecodingError) << std::endl;
        return std::nullopt;
    }

    /* Seem this texture was not loaded yet. Store it in GPU memory. */
    auto textureID = storeInGPUMemory(image, width, height);
    if(!textureID) {
        std::cerr << "Error storing image " << absolute_path << " in GPU memory" << std::endl;
        return std::nullopt;
    }

    /* Data successfully stored */
    image.clear();

    /* Check name */
    std::string textureName = "Texture_" + std::to_string((*textureID).value);
    if(!name.empty()) {
        textureName = name;
    }

    /* Build final texture data */
    TextureData texture_data(absolute_path, width, height, div_w, div_h, *textureID, textureName);

    /* Store in set */
    textureDatas.push_back(texture_data);

    std::cout << "Texture loaded successfully: " << texture_data.id << std::endl;
    return texture_data.id;
}

std::optional<MapData> Loader::loadMapData(World& world, const std::string& mapName) {
    /* Get absolute path to the map */
    const auto mapAbsolutePath = getMapAbsolutePath(mapName);

    /* Load map file with pugixml */
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(mapAbsolutePath.c_str());
    if(!result) {
        std::cerr << "Error loading map file: " << result.description() << std::endl;
        return std::nullopt;
    }

    /* Get map node and validate */
    auto mapNode = doc.child("map");
    if(!mapNode) {
        std::cerr << "Error loading map file: no map node in map file" << std::endl;
        return std::nullopt;
    }

    /* Extract map info */
    auto mapInfoOption = getMapInfo(mapNode);
    if(!mapInfoOption.has_value()) {
        std::cerr << "Error loading map file: invalid map info" << std::endl;
        return std::nullopt;
    }
    auto mapInfo = mapInfoOption.value();

    /* Initialize MapData - need to fill TilesetsData next */
    MapData mapData(mapAbsolutePath, mapInfo["mapWidth"], mapInfo["mapHeight"], mapInfo["tileWidth"], mapInfo["tileHeight"]);

    /**
     * Extract tileset info - global tiles ids with their source file relative path.
     */
    auto tilesetsInfo = getTilesetsInfo(mapNode);
    if(tilesetsInfo.empty()) {
        std::cerr << "Error loading map file: invalid tilesets info" << std::endl;
        return std::nullopt;
    }

    /* Load each tileset data */
    for(const auto& tilesetInfo : tilesetsInfo) {
        auto firstGid = std::get<0>(tilesetInfo);
        auto lastGid = std::get<1>(tilesetInfo);
        auto tilesetRelativePath = std::get<2>(tilesetInfo);

        auto tilesetData = loadTilesetData(firstGid, lastGid, mapAbsolutePath, tilesetRelativePath, mapData.tileWidth, mapData.tileHeight);
        if(!tilesetData.has_value()) {
            std::cerr << "Error loading map file: invalid tileset data" << std::endl;
            return std::nullopt;
        }

        /* Store tileset data */
        mapData.tilesetsData.push_back(tilesetData.value());
    }

    std::cout << mapData << std::endl;
    std::cout << "Map file loaded successfully" << std::endl;
    return mapData;
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
std::optional<TilesetData> Loader::loadTilesetData(int firstGid, std::optional<int> lastGid, const std::string& mapPath, const std::string& tilesetRelativePath, const int mapTileWidth, const int mapTileHeight) {
    const auto absuluteTilesetPath = getTilesetAbsolutePath(mapPath, tilesetRelativePath);
    // std::cout << "Loading tileset data from: " << absuluteTilesetPath << std::endl;

    /* Load tileset file with pugixml */
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(absuluteTilesetPath.c_str());
    if(!result) {
        std::cerr << "Error loading tileset file: " << result.description() << std::endl;
        return std::nullopt;
    }

    /* Get tileset node and validate */
    auto tilesetNode = doc.child("tileset");
    if(!tilesetNode) {
        std::cerr << "Error loading tileset file: no tileset node in tileset file" << std::endl;
        return std::nullopt;
    }

    /* Get tileset attributes */
    auto tilesetNameAttrib = tilesetNode.attribute("tilewidth");
    auto tileWidthAttrib = tilesetNode.attribute("tilewidth");
    auto tileHeightAttrib = tilesetNode.attribute("tileheight");
    auto tileCountAttrib = tilesetNode.attribute("tilecount");
    auto columnsAttrib = tilesetNode.attribute("columns");

    /* Check if tileset has required attibutes */
    if(!tilesetNameAttrib || !tileWidthAttrib || !tileHeightAttrib || !tileCountAttrib || !columnsAttrib) {
        std::cerr << "Error loading tileset file: no tileset name, tile size, tile count or columns" << std::endl;
        return std::nullopt;
    }

    /* Get attibutes values */
    std::string tilesetName = tilesetNode.attribute("name").as_string("");
    int tileWidth = tilesetNode.attribute("tilewidth").as_int(-1);
    int tileHeight = tilesetNode.attribute("tileheight").as_int(-1);
    int tileCount = tilesetNode.attribute("tilecount").as_int(-1);
    int columns = tilesetNode.attribute("columns").as_int(-1);

    /* Validate retrived values */
    if(tilesetName.empty() || tileWidth <= 0 || tileHeight <= 0 || tileCount <= 0 || columns <= 0) {
        std::cerr << "Error loading tileset file: invalid tileset name, tile size, tile count or columns" << std::endl;
        return std::nullopt;
    }

    /* Get tileset image node and validate */
    auto imageNode = tilesetNode.child("image");
   if(!imageNode) {
        std::cerr << "Error loading tileset file: no image node in tileset file" << std::endl;
        return std::nullopt;
    }

    /* Get image attributes */
    auto imageSourceAttrib = imageNode.attribute("source");
    auto imageWidthAttrib = imageNode.attribute("width");
    auto imageHeightAttrib = imageNode.attribute("height");

    /* Check if image has required attibutes */
    if(!imageSourceAttrib || !imageWidthAttrib || !imageHeightAttrib) {
        std::cerr << "Error loading tileset file: no image source, width or height" << std::endl;
        return std::nullopt;
    }

    /* Get attibutes values */
    std::string imageSource = imageNode.attribute("source").as_string("");
    int imageWidth = imageNode.attribute("width").as_int(-1);
    int imageHeight = imageNode.attribute("height").as_int(-1);

    /* Validate retrived values */
    if(imageSource.empty() || imageWidth <= 0 || imageHeight <= 0) {
        std::cerr << "Error loading tileset file: invalid image source, width or height" << std::endl;
        return std::nullopt;
    }

    auto absoluteTilesetImagePath = getTilesetImageAbsolutePath(absuluteTilesetPath, imageSource);
    std::cout << "Loading tileset image from: " << absoluteTilesetImagePath << std::endl;

    /* Store in TilesetData */
    TilesetData tilesetData(firstGid, lastGid, absuluteTilesetPath, absoluteTilesetImagePath, imageWidth, imageHeight, columns, tileCount / columns, tileWidth, tileHeight);

    /* Load tileset image */
    auto tilesetIdOption = loadTextureFromAbsolutePath(absoluteTilesetImagePath, tileWidth, tileHeight, tilesetName);
    if(!tilesetIdOption.has_value()) {
        std::cerr << "Error loading tileset file: invalid tileset image" << std::endl;
        return std::nullopt;
    }
    tilesetData.textureID = tilesetIdOption.value();

    /* Fill tileset data with tiles data */
    auto tilesLoadingResult = loadTilesData(tilesetNode, tilesetData, mapTileWidth, mapTileHeight);
    if(!tilesLoadingResult) {
        std::cerr << "Error loading tileset file: loading tiles data failed" << std::endl;
        return std::nullopt;
    }

    return tilesetData;
}

bool Loader::loadTilesData(const pugi::xml_node& mapNode, TilesetData& tilesetData, const int mapTileWidth, const int mapTileHeight) {

    /* Iterate over tiles nodes */
    for(auto tileNode : mapNode.children("tile")) {
        /* Get tile attributes */
        auto tileIdAttrib = tileNode.attribute("id");

        /* Check if tile has required attibutes */
        if(!tileIdAttrib) {
            std::cerr << "Error loading tileset file: no tile id" << std::endl;
            return false;
        }

        /* Get attibutes values */
        int tileId = tileNode.attribute("id").as_int(-1);

        /* Validate retrived values */
        if((tileId < 0) && !tilesetData.validateLID(tileId)) {
            std::cerr << "Error loading tileset file: invalid tile id" << std::endl;
            return false;
        }
        
        TileData tileData(tileId);

        /* Get tile animation node */
        auto animationNode = tileNode.child("animation");
        if(animationNode) {
            /* Iterate over animation frames */
            for(auto frameNode : animationNode.children("frame")) {
                /* Get frame attributes */
                auto tileIdAttrib = frameNode.attribute("tileid");
                auto durationAttrib = frameNode.attribute("duration");

                /* Check if frame has required attibutes */
                if(!tileIdAttrib || !durationAttrib) {
                    std::cerr << "Error loading tileset file: no frame tileid or duration" << std::endl;
                    return false;
                }

                /* Get attibutes values */
                int tileId = frameNode.attribute("tileid").as_int(-1);
                int duration = frameNode.attribute("duration").as_int(-1);

                /* Validate retrived values */
                if(((tileId < 0) && !tilesetData.validateLID(tileId)) || duration < 0) {
                    std::cerr << "Error loading tileset file: invalid frame tileid or duration" << std::endl;
                    return false;
                }

                /* Store in TilesetData */
                tileData.animationInterval = duration; // all frames has the same duration
                tileData.animationFramesLIDs.push_back(tileId);
            }
        }

        /* Get tile objectgroup node */
        auto collisionsRectsNode = tileNode.child("objectgroup");
        if(collisionsRectsNode) {
            /* Iterate over collision rects */
            for(auto collisionRectNode : collisionsRectsNode.children("object")) {
                /* Get object attributes */
                auto xAttrib = collisionRectNode.attribute("x");
                auto yAttrib = collisionRectNode.attribute("y");
                auto widthAttrib = collisionRectNode.attribute("width");
                auto heightAttrib = collisionRectNode.attribute("height");

                /* Check if object has required attibutes */
                if(!xAttrib || !yAttrib || !widthAttrib || !heightAttrib) {
                    std::cerr << "Error loading tileset file: no object x, y, width or height" << std::endl;
                    return false;
                }

                /* Get attibutes values */
                int x = collisionRectNode.attribute("x").as_int(-1);
                int y = collisionRectNode.attribute("y").as_int(-1);
                int width = collisionRectNode.attribute("width").as_int(-1);
                int height = collisionRectNode.attribute("height").as_int(-1);

                /* Validate retrived values */
                if(x < 0 || y < 0 || width < 0 || height < 0) {
                    std::cerr << "Error loading tileset file: invalid object x, y, width or height" << std::endl;
                    return false;
                }

                /* Store in TilesetData */
                float xScale = static_cast<float>(tilesetData.tileWidth) / static_cast<float>(mapTileWidth);
                float yScale = static_cast<float>(tilesetData.tileHeight) / static_cast<float>(mapTileHeight);

                float rectBaseX = static_cast<float>(x) / static_cast<float>(tilesetData.tileWidth);
                float rectBaseY = static_cast<float>(y) / static_cast<float>(tilesetData.tileHeight);
                float rectBaseW = static_cast<float>(width) / static_cast<float>(tilesetData.tileWidth);
                float rectBaseH = static_cast<float>(height) / static_cast<float>(tilesetData.tileHeight);

                auto collisionRect = Rect2F(
                    xScale * rectBaseX,
                    yScale * rectBaseY,
                    xScale * rectBaseW,
                    yScale * rectBaseH
                );
                tileData.collisionRects.push_back(collisionRect);
            }
        }

        /* Store tile data */
        tilesetData.tilesData.push_back(tileData);
    }

    return true;
}

std::string Loader::getTilesetAbsolutePath(const std::string& mapPath, const std::string& tilesetRelativePath) {
    /* TilesetRelativePath is in relation to map containing this dir */
    std::string mapDir = IO::get_containing_dir(mapPath);
    std::string absoluteTilesetPath = IO::get_absolute_path(mapDir + "\\" + tilesetRelativePath);
    return absoluteTilesetPath;
}

std::string Loader::getTilesetImageAbsolutePath(const std::string& tilesetAbsolutePath, const std::string& imageName) {
    /* absoluteTilesetImagePath is in the same dir as  tilesetAbsolutePath */
    std::string tilesetDir = IO::get_containing_dir(tilesetAbsolutePath);
    std::string absoluteTilesetImagePath = IO::get_absolute_path(tilesetDir + "\\" + imageName);
    return absoluteTilesetImagePath;
}

std::string Loader::getMapAbsolutePath(const std::string& mapName) {
    std::string mapDir = std::string(Settings::Resources::DATA_PATH)
        + std::string(Settings::Resources::MAPS_DIR);
    std::string absoluteMapPath = IO::get_absolute_path(mapDir + mapName);
    return absoluteMapPath;
}


bool Loader::buildWorld(World& world, const std::string mapName, const MapData& mapData) {
    /* Get absolute path to the map */
    const auto mapAbsolutePath = getMapAbsolutePath(mapName);

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

    /* Iterate over all lavers called group in tmx file */
    for(auto groupNode : mapNode.children("group")) {
        
        /* Get group attributes */
        auto nameAttrib = groupNode.attribute("name");

        /* Check if group has required attibutes */
        if(!nameAttrib) {
            std::cerr << "Error loading map file: no group name" << std::endl;
            return false;
        }

        /* Get attibutes values */
        std::string groupName = groupNode.attribute("name").as_string("");

        /* Validate retrived values */
        if(groupName.empty()) {
            std::cerr << "Error loading map file: invalid group name" << std::endl;
            return false;
        }

        std::cout << " + groupName: " << groupName << std::endl;

        /* Iterate over all layers in group */
        for(auto layerNode : groupNode.children("layer")) {

            /* Get group attributes */
            auto layerNameAttrib = layerNode.attribute("name");

            /* Check if group has required attibutes */
            if(!layerNameAttrib) {
                std::cerr << "Error loading map file: no layer name" << std::endl;
                return false;
            }

            /* Get attibutes values */
            std::string layerName = layerNode.attribute("name").as_string("");

            /* Validate retrived values */
            if(layerName.empty()) {
                std::cerr << "Error loading map file: invalid layer name" << std::endl;
                return false;
            }

            std::cout << "   * " << layerName << std::endl;

            /* Get layer data node */
            auto layerDataNode = layerNode.child("data");
            if(!layerDataNode) {
                std::cerr << "Error loading map file: no layer data" << std::endl;
                return false;
            }

            /* Get layer data */
            std::string layerData = layerDataNode.text().as_string("");

            /* Validate retrived values */
            if(layerData.empty()) {
                std::cerr << "Error loading map file: invalid layer data" << std::endl;
                return false;
            }

            /* Build layer data */
            std::vector<int> layerDataIndices;
            std::stringstream ss(layerData);
            std::string token;
            while(std::getline(ss, token, ',')) {
                layerDataIndices.push_back(std::stoi(token));
            }

            /* Validate retrived values */
            if(layerDataIndices.empty()) {
                std::cerr << "Error loading map file: invalid layer data" << std::endl;
                return false;
            }

            std::cout << "     - layerDataIndices.size(): " << layerDataIndices.size() << std::endl;

            /* Append layer */
            auto appendingLayerResult = appendWorldLayer(world, mapData, layerDataIndices);
            if(!appendingLayerResult) {
                std::cerr << "Error loading map file: invalid layer data" << std::endl;
                return false;
            }
#ifdef USE_ONLY_0_LAYER
            break;
#endif
        }
        /* Finally */
        
#ifdef USE_ONLY_0_GROUP
        break;
#endif
    }

    return true;
}

bool Loader::appendWorldLayer(World& world, const MapData& mapData, const std::vector<int> layerDataIndices) {
    const float regularTileWidth = static_cast<float>(mapData.tileWidth);
    const float regularTileHeight = static_cast<float>(mapData.tileHeight);

    int tileIndex{0};
    float tileX{0};
    float tileY{0};
    for(const auto tileGID: layerDataIndices) {
        /* Check empty spaces */
        if(tileGID == 0) {
            ++tileIndex;
            continue;
        }

        /* Calculate world location */
        tileX = static_cast<float>(tileIndex % mapData.width);
        tileY = static_cast<float>(tileIndex / mapData.width);

        /* Build tile */
        Entity *someTile = new Entity();

        auto locationCmp = new LocationComponent(someTile);
        locationCmp->worldRect.top_left.x = tileX;
        locationCmp->worldRect.top_left.y = tileY;
        locationCmp->worldRect.size.w = 1.0F;
        locationCmp->worldRect.size.h = 1.0F;
        someTile->addComponent(locationCmp);

        /* Retrive TilesetData corresponding to GID */
        auto tilesetDataOption = mapData.getTilesetDataCorrespondingToGID(tileGID);
        if(!tilesetDataOption.has_value()) {
            std::cerr << "Error getting TilesetData: invalid tile GID " << tileGID << std::endl;
            return false;
        }

        auto tilesetData = tilesetDataOption.value();
        
        /* Size fix for larger objects */
        locationCmp->worldRect.size.w = static_cast<float>(tilesetData.tileWidth) / regularTileWidth;
        locationCmp->worldRect.size.h = static_cast<float>(tilesetData.tileHeight) / regularTileHeight;

        /* Get TileData by GID */
        auto tileDataOption = tilesetData.getTileDataByGID(tileGID);
        if(!tileDataOption.has_value()) {
            std::cerr << "Error getting TileData: invalid tile GID " << tileGID << std::endl;
            return false;
        }
        auto tileData = tileDataOption.value();

        /* Get Texture data from Tileset and Tile Data */
        TextureID textureID = tilesetData.textureID;
        auto textureCmp = new TextureComponent(someTile, locationCmp, textureID, tileData.tileLID);
        someTile->addComponent(textureCmp);

        /* Check if tile has animation */
        if(tileData.hasAnimation()) {
            auto animationCmp = new AnimationComponent(someTile, textureCmp, tileData.animationInterval);
            for(const auto& animationFrameLID : tileData.animationFramesLIDs) {
                animationCmp->appendIndex(animationFrameLID);
            }
            animationCmp->setActive(true);
            someTile->addComponent(animationCmp);
        }

        /* Check if has collision rects */
        if(tileData.hasCollisionRects()) {
            auto collisionCmp = new CollisionComponent(someTile, locationCmp);
            for(const auto& collisionRect : tileData.collisionRects) {
                collisionCmp->appendCollidionRect(collisionRect);
            }
            someTile->addComponent(collisionCmp);
        }

        /* Append new tile to world */
        world.entities.push_back(someTile);
        ++tileIndex;
    }

    std::cout << "Layer appended successfully. Total entities count: " << world.entities.size() << std::endl;
    return true;
}




