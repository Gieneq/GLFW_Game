#pragma once
#include <vector>
#include "LayerData.h"
#include "Settings.h"
#include "Maths.h"
#include <string>

class FloorNumber {
public:
    FloorNumber() = default;
    FloorNumber(int level) : level{level} {}
    
    int get_level() const { 
        return level; 
    }

    FloorNumber from_level(int level) {
        return FloorNumber{level};
    }

    FloorNumber from_string(const std::string& level) {
        return FloorNumber{std::stoi(level)};
    }

    size_t operator()(const FloorNumber& floor) const {
        return std::hash<int>()(floor.level);
    }
    
    bool operator==(const FloorNumber& other) const {
        return level == other.level;
    }

    bool operator<(const FloorNumber& other) const {
        return level < other.level;
    }

private:
    int level{0};
};


// z worlda sciaga info o rozmiarze - wszystkie maja taki samrozmiar

class WorldData;

class FloorData {
public:
    FloorData() = default;
    FloorData(int floor_number, WorldData* parent) : floor_number{floor_number}, parent_world_data{parent} {}
    FloorNumber floor_number{};
    std::vector<LayerData> layers;
    WorldData* parent_world_data{nullptr};
    // int get_width() const { return parent_world_data ? parent_world_data->get_width() : Settings::Map::WIDTH; }
    // int get_height() const { return parent_world_data ? parent_world_data->get_height() : Settings::Map::HEIGHT; }
    
};


