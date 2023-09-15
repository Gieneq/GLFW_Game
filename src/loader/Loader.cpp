#include "Loader.h"

#include <algorithm>
#include <sstream>

#include "GraphicsComponent.h"
#include "AnimationComponent.h"
#include "MovementComponent.h"
#include "ControllableComponent.h"
#include "CollisionComponents.h"

#include "Timers.h"
#include "IO.h"
#include "Settings.h"


bool Loader::loadAssets() {
    std::cout << "Loading assets data..." << std::endl;
    
    /* Load test tiles texture */
    auto loadingResult = Loader::getLoader().loadTextureFromAssets("tiles.png", 8, 8, 64, 64, "some_tiles");
    if(!loadingResult) {
        std::cerr << "Error loading texture from assets" << std::endl;
        return false;
    }
    return true;
}

bool Loader::loadWorld(World& world) {
    std::cout << "Loading world data..." << std::endl;

    /**
     * Load map data from files.
     * Tiles are not built yet.
     * It is used to build tiles and populate world.
     */
    const std::string mapFileName = "testisland.tmx";

    auto mapLoadingResul = loadMapDataFromTMXFile(world, mapFileName);
    if(!mapLoadingResul) {
        std::cerr << "Error loading map data" << std::endl;
        return false;
    }

    /* Use mapData to build World */
    auto worldBuildResult = buildWorldFromMapData(world);
    return true;
}

bool Loader::loadPlayer(World& world) {
    std::cout << "Loading player data..." << std::endl;
    Player* player = &world.player;
    
    /* Player as all Entities has to have pointer to it's Elevation */
    try {
        auto playersElevation = player->getContainingElevationOrThrow();
    } catch (std::runtime_error&) {
        std::cout << "Player has no Elevation, try setting bottom [0]" << std::endl;
        try {
            auto bottomElevation = world[0];

            /* Player has to be added to the Elevation */
            player->setElevation(bottomElevation);
            bottomElevation->registetedEntityOrThrow(player);

        } catch (std::out_of_range& e) {
            std::cout << "World has no Elevations: " << e.what() << std::endl;
            return false;
        } catch (std::invalid_argument& e) {
            std::cout << "Error adding player to Elevation: " << e.what() << std::endl;
            return false;
        }
    }

    /* Set starting position */
    player->getCuboid().value().x() = Settings::Player::INITIAL_X;
    player->getCuboid().value().y() = Settings::Player::INITIAL_Y;
    const int playerElevationIndex = Settings::Player::INITIAL_ELEVATION_IDX;
    player->getCuboid().value().z() = Settings::Player::INITIAL_Z;

    /* Try adding texture to player */
    auto playersTextureIDOption = Loader::getLoader().getTextureDataByName("some_tiles");

    if(playersTextureIDOption.has_value()) {
        auto playersTextureData = playersTextureIDOption.value();
        /* Texture */
        std::cout << "Player has texture" << std::endl;
        //todo need refinements 1x1 size not obvious
        auto textureCmp = player->addTextureComponent(playersTextureData->getTextureID(), 0.0F, 0.0F, 1.0F, 1.0F);
        textureCmp->setTilesetIndex(0);
    } 
    else {
        /* Color */
        std::cout << "Player has no texture, add placeholder color" << std::endl;
        auto colorCmp = player->addColorComponent(0.0F, 0.0F, 1.0F, 1.0F);
        colorCmp->r = 1.0F;
        colorCmp->g = 0.0F;
        colorCmp->b = 1.0F;
    }

    /* Movement option */
    auto movementCmp = player->addMovementComponent(Settings::Player::INITIAL_SPEED);

    /* WSAD to control player */
    auto controllableCmpOption = player->addControllableComponent();
    if(!controllableCmpOption.has_value()) {
        std::cerr << "Error adding controllable component to player" << std::endl;
        return false;
    }

    /* Collision detector */
    //todo - applying some shift in box, cause problems. Seems detection box is not used in elevation change, but
    //entitiy box is used.
    auto collisionDetectorCmpOption = player->addCollisionDetectorComponent(Cuboid6F(0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F));
    if(!collisionDetectorCmpOption.has_value()) {
        std::cerr << "Error adding collision detector component to player" << std::endl;
        return false;
    }

    world.moveEntityToElevationOrThrow(player, playerElevationIndex);

    return true;
}


bool Loader::loadMapDataFromTMXFile(World& world, const std::string& mapName) {
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

    /* Extract map info */
    auto mapInfoOption = getMapInfoFromMapNode(mapNode);
    if(!mapInfoOption.has_value()) {
        std::cerr << "Error loading map file: invalid map info" << std::endl;
        return false;
    }
    auto mapInfo = mapInfoOption.value();

    /* Initialize MapData */
    mapData.create(mapAbsolutePath, mapInfo["mapWidth"], mapInfo["mapHeight"], mapInfo["tileWidth"], mapInfo["tileHeight"]);

    /**
     * Extract tileset info - global tiles ids with their source file relative path.
     */
    auto tilesetsInfo = getTilesetsInfoListFromMapNode(mapNode);
    if(tilesetsInfo.empty()) {
        std::cerr << "Error loading map file: invalid tilesets info" << std::endl;
        return false;
    }

    /* Load each tileset data */
    for(const auto& tilesetInfo : tilesetsInfo) {
        auto firstGid = std::get<0>(tilesetInfo);
        auto lastGid = std::get<1>(tilesetInfo);
        auto tilesetRelativePath = std::get<2>(tilesetInfo);

        auto tilesetDataOption = loadTilesetDataFromTSXFile(firstGid, lastGid, tilesetRelativePath);
        if(!tilesetDataOption.has_value()) {
            std::cerr << "Error loading map file: invalid tileset data" << std::endl;
            return false;
        }
    }
    std::cout << mapData << std::endl;
    std::cout << "Map file loaded successfully" << std::endl;
    return true;
}

std::optional<std::map<std::string, int>> Loader::getMapInfoFromMapNode(const pugi::xml_node& mapNode) {
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

std::vector<std::tuple<int, std::optional<int>, std::string>> Loader::getTilesetsInfoListFromMapNode(const pugi::xml_node& mapNode) {
    
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
std::optional<TilesetData*> Loader::loadTilesetDataFromTSXFile(int firstGid, std::optional<int> lastGid, const std::string& tilesetRelativePath) {
    const auto absuluteTilesetPath = getTilesetAbsolutePath(mapData.mapAbsolutePath, tilesetRelativePath);
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
    auto tilesetData = mapData.appendTilesetData(firstGid, lastGid);
    if(!tilesetData) {
        std::cerr << "Error loading tileset file: invalid tileset data" << std::endl;
        return std::nullopt;
    }

    tilesetData->tilesetAbsolutePath = absuluteTilesetPath;
    tilesetData->tilesetImageAbsolutePath = absoluteTilesetImagePath;
    tilesetData->columns = columns;
    tilesetData->rows = tileCount / columns;
    tilesetData->tileWidth = tileWidth;
    tilesetData->tileHeight = tileHeight;

    /* Load tileset image */
    auto tilesetIdOption = loadTextureFromAbsolutePath(absoluteTilesetImagePath, tileWidth, tileHeight, tileCount / columns, columns, tilesetName);
    if(!tilesetIdOption.has_value()) {
        std::cerr << "Error loading tileset file: invalid tileset image" << std::endl;
        return std::nullopt;
    }
    tilesetData->textureID = tilesetIdOption.value();

    /* Fill tileset data with tiles data */
    auto tilesLoadingResult = getTilesDataFromTilesetNode(tilesetNode, tilesetData);
    if(!tilesLoadingResult) {
        std::cerr << "Error loading tileset file: loading tiles data failed" << std::endl;
        return std::nullopt;
    }

    return tilesetData;
}

#define CAN_HAS_COLLISON 1
bool Loader::getTilesDataFromTilesetNode(const pugi::xml_node& mapNode, TilesetData* tilesetData) {

    /* Iterate over tiles nodes */
    for(auto tileNode : mapNode.children("tile")) {
        /* Get tile attributes */
       
        /* Get ID */
        const auto tileIdOption = getTileIdFromTileNode(tileNode);
        if(!tileIdOption.has_value()) {
            std::cerr << "Error loading tileset file: invalid tile id" << std::endl;
            return false;
        }
        const int tileId = tileIdOption.value();
        
        auto tileData = tilesetData->appendTileData(tileId);

        /* Retrive and apply animation if any */
        auto animationOption = retriveTileDataAnimationFromTileNode(tileNode, tileData);
        if(animationOption.has_value()) {
            if(animationOption.value() == false) {
                std::cerr << "Error loading tileset file: invalid tile animation" << std::endl;
                return false;
            }
        }


        /* Retrive stairs properties */
        auto stairsOption = retriveTileDataStairsFromTileNode(tileNode, tileData);
        if(stairsOption.has_value()) {
            if(stairsOption.value() == false) {
                std::cerr << "Error loading tileset file: invalid stairs property" << std::endl;
                return false;
            }
        }

        /* Stairs removes any collision options */
#if CAN_HAS_COLLISON == 1
        bool canHaseCollision = !stairsOption.has_value();
#else
        bool canHaseCollision = false;
#endif
        if(canHaseCollision) {
            /* Retrive and apply collision if any */
            auto collisionOption = retriveTileDataCollisionFromTileNode(tileNode, tileData);
            if(collisionOption.has_value()) {
                if(collisionOption.value() == false) {
                    std::cerr << "Error loading tileset file: invalid tile collision" << std::endl;
                    return false;
                }
            }
        }

    }

    return true;
}

std::optional<int> Loader::getTileIdFromTileNode(const pugi::xml_node& tileNode) {
    /* Get tile attributes */
    auto tileIdAttrib = tileNode.attribute("id");

    /* Check if tile has required attibutes */
    if(!tileIdAttrib) {
        std::cerr << "Error loading tileset file: no tile id" << std::endl;
        return std::nullopt;
    }

    /* Get attibutes values */
    int tileId = tileNode.attribute("id").as_int(-1);

    /* Validate retrived values */
    if(tileId < 0) {
        std::cerr << "Error loading tileset file: invalid tile id" << std::endl;
        return std::nullopt;
    }

    return tileId;
}

std::optional<bool> Loader::retriveTileDataAnimationFromTileNode(const pugi::xml_node& tileNode, TileData* tileData) {
        /* Get tile animation node */
        auto animationNode = tileNode.child("animation");

        if(!animationNode) {
            /* No animation - nothing to return */
            return std::nullopt;
        }

        TilesetData* tilesetData = tileData->containingTilesetData;

        /* Iterate over animation frames */
        for(auto frameNode : animationNode.children("frame")) {
            /* Get frame attributes */
            auto tileIdAttrib = frameNode.attribute("tileid");
            auto durationAttrib = frameNode.attribute("duration");

            /* Check if frame has required attibutes */
            if(!tileIdAttrib || !durationAttrib) {
                std::cerr << "Error loading tileset file: no frame tileid or duration" << std::endl;
                return std::make_optional(false);
            }

            /* Get attibutes values */
            int tileId = frameNode.attribute("tileid").as_int(-1);
            int duration = frameNode.attribute("duration").as_int(-1);

            /* Validate retrived values */
            if(((tileId < 0) && !tilesetData->validateLID(tileId)) || duration < 0) {
                std::cerr << "Error loading tileset file: invalid frame tileid or duration" << std::endl;
                return std::make_optional(false);
            }

            /* Store in TilesetData */
            tileData->animationInterval = duration; // all frames has the same duration
            tileData->animationFramesLIDs.push_back(tileId);
        }
    return std::make_optional(true);
}


std::optional<bool> Loader::retriveTileDataStairsFromTileNode(const pugi::xml_node& tileNode, TileData* tileData) {
    auto propertiesNode = tileNode.child("properties");

    if(!propertiesNode) {
        /* No properties - nothing to return */
        return std::nullopt;
    }

    /* 
     * Stairs related properties: 
     *  - stairs_min
     *  - stairs_max
     */

    /* Get stairs properties */
    int stairsMin = -1;
    int stairsMax = -1;
    std::string stairsHeading = "up"; //todo

    for(auto propertyNode : propertiesNode.children("property")) {
        auto propertyName = propertyNode.attribute("name");
        auto propertyValue = propertyNode.attribute("value");
        //todo add heaing

        if(!propertyName || !propertyValue) {
            std::cerr << "Error loading tileset file: no property name or value" << std::endl;
            return std::make_optional(false);
        }

        std::string name = propertyName.as_string("");
        if(name == "stairs_min") {
            stairsMin = propertyValue.as_int(-1);
        }
        else if(name == "stairs_max") {
            stairsMax = propertyValue.as_int(-1);
        }

        //todo add heaing parsing
    }

    /* Check if has any stairs related properties */
    if(stairsMin < 0 || stairsMax < 0 || stairsHeading.empty()) {
        return std::nullopt;
    }

    /* Yup - it is stairs tile */

    /* Ensure stairsMax stairsMin */
    if(stairsMax < stairsMin) {
        std::swap(stairsMax, stairsMin);
    }

    /* Validate heading : to lowercase, compare to up, down, left, right */
    std::string stairsHeadingLower = stairsHeading;
    std::transform(stairsHeadingLower.begin(), stairsHeadingLower.end(), stairsHeadingLower.begin(), ::tolower);
    if(stairsHeadingLower != "up" && stairsHeadingLower != "down" && stairsHeadingLower != "left" && stairsHeadingLower != "right") {
        std::cerr << "Error loading tileset file: invalid stairs heading" << std::endl;
        return std::make_optional(false);
    }



    /**
     * Store in TilesetData
     * With known sumber of steps generate collision cuboid with
     * height changing from min to max.
     */

    const auto stepsCount = Settings::Systems::Collisions::STAIRS_STEPS_COUNT;
    const auto heightDifference = static_cast<float>(stairsMax - stairsMin) / 20.0F;
    const auto startingHeight = static_cast<float>(stairsMin) / 20.0F;
    const auto stepDiff = heightDifference / static_cast<float>(stepsCount);
    // const auto stepHeight = 7.0F / 20.0F;
    const auto stairsWidth = 1.0F;
    const auto stairsHeight = 1.8F / stepsCount;

    for(int i = 0; i < stepsCount; ++i) {
        const float proportion = static_cast<float>(i) / static_cast<float>(stepsCount);
        const float invProportion = 1.0F - proportion;

        auto collisionCuboid = Cuboid6F(
            0.0F,
            0.0F + i * stairsHeight,
            0.0F,
            stairsWidth,
            stairsHeight,
            startingHeight + (stepsCount-i) * stepDiff
        );

        // #error
        //partialy working
        // todo:
        /**
         * 1. depthPx seems not usable
         * 2. find topmost collision if multiple collision boxes found
         * but! pas only one to collisionTDetectorComponent
        */
    //    #error

        tileData->collisionCuboids.push_back(collisionCuboid);
        tileData->walkable = true;
        // tileData->depthPx = 7;
    }

    return std::make_optional(true);
}

std::optional<bool> Loader::retriveTileDataCollisionFromTileNode(const pugi::xml_node& tileNode, TileData* tileData) {
    /* Get tile objectgroup node */
    auto collisionsRectsNode = tileNode.child("objectgroup");
    if(!collisionsRectsNode) {
        /* No collision rects - nothing to return */
        return std::nullopt;
    }
    
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
            return std::make_optional(false);
        }

        /* Get attibutes values */
        int x = xAttrib.as_int(-1);
        int y = yAttrib.as_int(-1);
        int width = widthAttrib.as_int(-1);
        int height = heightAttrib.as_int(-1);

        /* Validate retrived values */
        if(x < 0 || y < 0 || width < 0 || height < 0) {
            std::cerr << "Error loading tileset file: invalid object x, y, width or height" << std::endl;
            return std::make_optional(false);
        }

        /* Store in TilesetData */
        auto collisionCuboid = Cuboid6F(
            static_cast<float>(x) / tileData->containingTilesetData->containingMapData->tileWidthBase,
            static_cast<float>(y) / tileData->containingTilesetData->containingMapData->tileHeightBase,
            0.0F,
            static_cast<float>(width) / tileData->containingTilesetData->containingMapData->tileWidthBase,
            static_cast<float>(height) / tileData->containingTilesetData->containingMapData->tileHeightBase,
            1.0F //will be modified by optional depth property
        );

        tileData->collisionCuboids.push_back(collisionCuboid);
    }

    /* Find additional properties - depth */
    auto propertiesNode = tileNode.child("properties");
    if(propertiesNode) {
        /* Has some properties */

        /* Find property with arribute name depth */
        auto propertyNode = propertiesNode.child("property");

        if(propertyNode) {
            auto nameAttrib = propertyNode.attribute("name");
            auto valueAttrib = propertyNode.attribute("value");

            if(nameAttrib && valueAttrib) {
                std::string name = nameAttrib.as_string("");
                if(name == "depth") {
                    tileData->walkable = true;
                    int value = valueAttrib.as_int(-1);
                    // tileData->depthPx = value;

                    /* Apply to all collision cuboids depths */
                    for(auto& collisionCuboid : tileData->collisionCuboids) {
                        collisionCuboid.d() = static_cast<float>(value) / static_cast<float>(Settings::Map::TILE_DEPTH);
                    }
                }
            }
        }    
    }

    return std::make_optional(true);
}

bool Loader::buildWorldFromMapData(World& world) {
    /* Get absolute path to the map */
    const auto mapAbsolutePath = mapData.mapAbsolutePath;

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

    int groupIdx = 0; // elevation index
    int layerIdx = 0;

    /* Iterate over all lavers called group in tmx file */
    for(auto groupNode : mapNode.children("group")) {

        /* Create Elevation inside world */
        auto recentTopElevation = world.createElevationOrThrow();
        std::cout << "Recently created elevation id: " << recentTopElevation->getIndex() << std::endl;
                
        /* Check index */
        if(recentTopElevation->getIndex() != groupIdx) {
            std::cerr << "Error loading map file: invalid group index" << std::endl;
            return false;
        }

        /* Iterate over all layers in group */
        for(auto layerNode : groupNode.children("layer")) {
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
            
            /* With layer index find entity type */
            Entity::Type newEntityType = (layerIdx < 2) ? Entity::Type::FLOOR : Entity::Type::STATIC;

            /* Append layer */
            auto fillingResult = fillElevationWithEntities(world, recentTopElevation, newEntityType, layerDataIndices);
            if(!fillingResult) {
                std::cerr << "Error loading map file: couldn't fill Elevation with Entities" << std::endl;
                return false;
            }

#ifdef USE_ONLY_0_LAYER
            break;
#endif
            ++layerIdx;
        }
        /* Finally */
        
#ifdef USE_ONLY_0_GROUP
        break;
#endif
    layerIdx = 0;
    ++groupIdx;
    }

    return true;
}


bool Loader::fillElevationWithEntities(World& world, Elevation* elevation, Entity::Type entityType, const std::vector<int> layerDataIndices) {
    int tileIndex{0};
    float tileX{0};
    float tileY{0};

    for(const auto tileGID: layerDataIndices) {
        /* Check empty spaces */
        if(tileGID == 0) {
            ++tileIndex;
            continue;
        }

        
        /* Set location in elevation space */
        tileX = static_cast<float>(tileIndex % mapData.getTotalWidth());
        tileY = static_cast<float>(tileIndex / mapData.getTotalWidth());

        /* Test block */
#ifdef USE_TEST_BLOCK
        if(!(tileX > 80 && tileX < 102 && tileY > 80 && tileY < 102)) {
            ++tileIndex;
            continue;
        }
#endif
        /* Retrive TilesetData corresponding to GID */
        auto tilesetDataOption = mapData.getTilesetDataCorrespondingToGID(tileGID);
        if(!tilesetDataOption.has_value()) {
            std::cerr << "Error getting TilesetData: invalid tile GID " << tileGID << std::endl;
            return false;
        }

        /* Got tileset data - proceed */
        auto tilesetData = tilesetDataOption.value();
        
        /* Build tile - register automatically to the world */
        Entity* tileEntity = nullptr;

        try {
            tileEntity = elevation->createEntityOrThrow(entityType);
        } catch (std::bad_alloc& e) {
            std::cerr << "Error creating Entity: " << e.what() << std::endl;
            return false;
        }



        /* Position */
        

        tileEntity->getCuboid().value().setTopLeft(
            tileX, 
            tileY - ((tilesetData->getTileRelativeHeightScale() > 1.0F) ? (tilesetData->getTileRelativeHeightScale() - 1.0F) : 0.0F), 
            0.0F
        );

        if(Settings::Resources::USE_VERTICAL_SHIFT) {
            const int elevationFixY = elevation->getIndex();
            tileEntity->getCuboid().value().y() += elevationFixY;
        }

        /* Size */
        tileEntity->getCuboid().value().setSize(
            1.0F * tilesetData->getTileRelativeWidthtScale(), 
            1.0F * tilesetData->getTileRelativeHeightScale(), 
            0.0F
        );

        /* Get TileData by GID */
        auto tileDataOption = tilesetData->getTileDataByGID(tileGID);
        if(!tileDataOption.has_value()) {
            std::cerr << "Error getting TileData: invalid tile GID " << tileGID << std::endl;
            return false;
        }
        auto tileData = tileDataOption.value();

        /* Get Texture data from Tileset and Tile Data */
        auto textureID = tilesetData->getTextureID();

        auto textureCmp = tileEntity->addTextureComponent(
            textureID,
            0.0F, 
            0.0F, 
            1.0F * tilesetData->getTileRelativeWidthtScale(), 
            1.0F * tilesetData->getTileRelativeHeightScale()
        );
        textureCmp->setTilesetIndex(tileData->tileLID);

        /* Check if tile has animation */
        if(tileData->hasAnimation()) {
            auto animationCmpOption = tileEntity->addAnimationComponent(tileData->animationInterval);
            if(!animationCmpOption.has_value()) {
                std::cerr << "Error creating AnimationComponent" << std::endl;
                return false;
            }
            auto animationCmp = animationCmpOption.value();
            for(auto animationFrameLID : tileData->animationFramesLIDs) {
                animationCmp->appendIndex(animationFrameLID);
            }
            animationCmp->setActive(true);
        }

        /* Check if has collision cuboids */
        if(tileData->hasCollisionRects()) {
            auto collisionCmp = tileEntity->addCollisionComponent();
            collisionCmp->allowWalkOn = tileData->walkable;

            for(auto collisionCuboidIt = tileData->collisionCuboids.begin(); collisionCuboidIt != tileData->collisionCuboids.end(); ++collisionCuboidIt) {
                auto collisionCuboid = *collisionCuboidIt;
                //hacky
                // collisionCuboid.size.d = static_cast<float>(tileData->depthPx) / static_cast<float>(mapData.getBaseTileHeight());
                collisionCmp->appendCollisionCuboid(collisionCuboid);
            }
        }

        /* Append new tile to world */
        ++tileIndex;
    }
    return true;
}




