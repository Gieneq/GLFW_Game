#pragma once
#include <vector>
#include "TileData.h"
#include "Settings.h"

class FloorData;

class LayerData {
public:
    int z_index{0};
    FloorData* parent_floor_data{nullptr};
    // int get_width() const { return parent_floor_data ? parent_floor_data->get_width() : Settings::Map::WIDTH; }
    // int get_height() const { return parent_floor_data ? parent_floor_data->get_height() : Settings::Map::HEIGHT; }
    
    std::vector<TileData> tiles;
};