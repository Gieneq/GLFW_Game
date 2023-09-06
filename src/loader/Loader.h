#pragma once

#include "pugixml.hpp"
#include "LoaderDatatypes.h"
#include "LoaderBase.h"

#include <string>
#include <optional>
#include <iostream>
#include <tuple>
#include <map>


#include "World.h"
#include "Maths.h"

/**
 * Group is Floor class - heigt of terrain
 * Layer is loading part of Group/Floor
*/

// #define BUILD_TESTWORLD 1
// #define USE_ONLY_0_GROUP 1
// #define USE_ONLY_0_LAYER 1



class Loader : public LoaderBase {
private:
    Loader() : LoaderBase() {}
    ~Loader() = default;
public:
    static Loader& getLoader() {
        static Loader loader;
        return loader;
    }

    bool loadAssets();
    bool loadWorld(World& world);
    bool loadPlayer(World& world);

    inline int getLoadedTilesetsCount() const { return mapData.getTilesetsCount(); }
    inline int getLoadedTilesDataCount() const { return mapData.getAllTileDatasCount(); }

private:
    /* Map Loading specific */
    bool loadMapDataFromTMXFile(World& world, const std::string& mapName);

    std::optional<std::map<std::string, int>> getMapInfoFromMapNode(const pugi::xml_node& mapNode);

    std::vector<std::tuple<int, std::optional<int>, std::string>> getTilesetsInfoListFromMapNode(const pugi::xml_node& mapNode);

    std::optional<TilesetData*> loadTilesetDataFromTSXFile(int firstGid, std::optional<int> lastGid, const std::string& tilesetRelativePath);

    bool getTilesDataFromTilesetNode(const pugi::xml_node& mapNode, TilesetData* tilesetData);

    /* TileData parsers */
    std::optional<int> getTileIdFromTileNode(const pugi::xml_node& tileNode);

    std::optional<bool> retriveTileDataAnimationFromTileNode(const pugi::xml_node& tileNode, TileData* tileData);

    std::optional<bool> retriveTileDataStairsFromTileNode(const pugi::xml_node& tileNode, TileData* tileData);

    std::optional<bool> retriveTileDataCollisionFromTileNode(const pugi::xml_node& tileNode, TileData* tileData);

    /* Other */
    bool buildWorldFromMapData(World& world);

    bool fillElevationWithEntities(World& world, Elevation* elevation, Entity::Type entityType, const std::vector<int> layerDataIndices);

    MapData mapData{};
};