#pragma once
#include "lodepng.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "World.h"
#include "TilesetData.h"
#include "TileId.h"
#include "TilesetRange.h"
#include "EntityRegister.h"
#include "EntityTemplate.h"
#include "TextureId.h"
#include "EntityGlobalId.h"
#include "TextureData.h"



class Loader {
private:
    Loader() = default;
    ~Loader() = default;
public:
    static Loader& get_loader() {
        static Loader loader;
        return loader;
    }

    TextureId load_image(std::string abs_path);
    TextureId load_image_from_resources(std::string res_path);
    bool register_texture_name(const std::string& name, const TextureId& id);

    bool has_image(TextureId key_texture_id) {
        return images.find(key_texture_id) != images.end();
    }

    TextureData& get_texture_data(TextureId key_texture_id) {
        return images[key_texture_id];
    }

    TextureId& get_texture_id(const std::string& name) {
        return texture_names[name];
    }

    // Tileset load_tileset(const Range_i& gids_range, const std::string& absolute_path_tls);

    void load_map(World& world, std::string map_name);

private:
    TextureData store_in_gpu_memory(std::vector<unsigned char>& pixels, int width, int height, std::string abs_path);

    std::unordered_map<TextureId, TextureData, TextureId> images{};
    std::unordered_map<std::string, TextureId> texture_names{};
    std::unordered_map<TextureId, TilesetData, TextureId> tilesets{};

};