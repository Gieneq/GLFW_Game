#pragma once
#include "EntityGlobalId.h"
#include "Settings.h"
#include "TileId.h"

class TilesetRange {
public:
    TilesetRange(int first_gid, int last_gid) : first_gid(first_gid), last_gid(last_gid) {}
    TilesetRange(EntityGlobalId first_gid, EntityGlobalId last_gid) : first_gid(first_gid), last_gid(last_gid) {}

    TilesetRange(int first_gid) : first_gid(first_gid), last_gid(Settings::Map::MAX_GID) {}
    TilesetRange(EntityGlobalId first_gid) : first_gid(first_gid), last_gid(Settings::Map::MAX_GID) {}

    EntityGlobalId first_gid;
    EntityGlobalId last_gid;

    bool operator==(const TilesetRange& other) const {
        return first_gid == other.first_gid && last_gid == other.last_gid;
    }

    bool is_in_range(EntityGlobalId id) const {
        return id.id >= first_gid.id && id.id <= last_gid.id;
    }

    TileId decode_entity_gid(EntityGlobalId gid) const {
        return TileId(gid.id - first_gid.id);
    }

    EntityGlobalId encode_entity_gid(TileId tile_id) const {
        return tile_id.id + first_gid.id;
    }

};