#pragma once
#include "TilesetRange.h"
#include "TextureId.h"


class TilesetData {
public:
    TilesetData(const TilesetRange& range, const TextureId& texture_id);
    TilesetRange range{0,0};
    TextureId texture_id{};
};