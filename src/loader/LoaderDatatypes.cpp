#include "LoaderDatatypes.h"



/* Tile Data */

std::vector<TileData*> TileData::getAnimationFramesTileDatas() {
    std::vector<TileData*> animationFramesTileDatas;
    for(auto animationFrameLID : animationFramesLIDs) {
        animationFramesTileDatas.push_back(containingTilesetData->getTileDataByLID(animationFrameLID));
    }
    return animationFramesTileDatas;
}

/* Tileset Data */

TileData* TilesetData::getTileDataByLID(int lid) {
    /* Check if this tileset can have corresponding LID */
    if(!validateLID(lid)) {
        return nullptr;
    }

    /* Has LID, find TileData in vector */
    for(auto& tileData : tileDatas) {
        if(tileData.tileLID == lid) {
            return &tileData;
        }
    }

    /* No tile data found - generate with lid */
    return appendTileData(lid);
}

std::optional<TileData*> TilesetData::getTileDataByGID(int gid) {
    /* Check if this tileset can have corresponding GID */
    auto lidOption = decodeGID(gid);
    if(!lidOption.has_value()) {
        return std::nullopt;
    }

    auto lid = lidOption.value();

    /* Has LID, find TileData in vector */
    for(auto& tileData : tileDatas) {
        if(tileData.tileLID == lid) {
            return &tileData;
        }
    }

    /* No tile data found - generate with lid */
    return appendTileData(lid);
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

TileData* TilesetData::appendTileData(int lid) {
    TileData tileData{lid};
    tileDatas.push_back(tileData);
    auto tileDataPtr = &tileDatas.back();
    tileDataPtr->containingTilesetData = this;

    return tileDataPtr;
}

float TilesetData::getTileRelativeWidthtScale() const {
    return static_cast<float>(tileWidth) / static_cast<float>(containingMapData->getBaseTileWidth());
}

float TilesetData::getTileRelativeHeightScale() const {
    return static_cast<float>(tileHeight) / static_cast<float>(containingMapData->getBaseTileHeight());
}


/* Map Data */

bool MapData::create(const std::string& mapAbsolutePath, int width, int height, int tileWidth, int tileHeight) {
    this->mapAbsolutePath = mapAbsolutePath;
    this->widthTotal = width;
    this->heightTotal = height;
    this->tileWidthBase = tileWidth;
    this->tileHeightBase = tileHeight;

    tilesetDatas.clear();

    return validate();
}

bool MapData::validate() const {
    if(mapAbsolutePath.empty()) {
        std::cerr << "MapData::validate() - mapAbsolutePath is empty!" << std::endl;
        return false;
    }

    if(widthTotal <= 0) {
        std::cerr << "MapData::validate() - width is invalid!" << std::endl;
        return false;
    }

    if(heightTotal <= 0) {
        std::cerr << "MapData::validate() - height is invalid!" << std::endl;
        return false;
    }

    if(tileWidthBase <= 0) {
        std::cerr << "MapData::validate() - tileWidth is invalid!" << std::endl;
        return false;
    }

    if(tileHeightBase <= 0) {
        std::cerr << "MapData::validate() - tileHeight is invalid!" << std::endl;
        return false;
    }

    return true;
}

std::optional<TilesetData*> MapData::getTilesetDataCorrespondingToGID(int gid) {
    for(auto& tilesetData : tilesetDatas) {
        auto tileDataOption = tilesetData.getTileDataByGID(gid);
        if(tileDataOption.has_value()) {
            return &tilesetData;
        }
    }
    return std::nullopt;
}


std::optional<TileData*> MapData::getTileDataCorrespondingToGID(int gid) {
    auto tilesetDataOption = getTilesetDataCorrespondingToGID(gid);
    if(!tilesetDataOption.has_value()) {
        return std::nullopt;
    }

    auto tilesetData = tilesetDataOption.value();
    return tilesetData->getTileDataByGID(gid);
}

TilesetData* MapData::appendTilesetData(int firstGID, std::optional<int> lastGID) {
    TilesetData tilesetData{firstGID, lastGID};
    tilesetDatas.push_back(tilesetData);
    auto tilesetDataPtr = &tilesetDatas.back();
    tilesetDataPtr->containingMapData = this;

    return tilesetDataPtr;
}



