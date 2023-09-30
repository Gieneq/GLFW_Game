import os
import xml.etree.ElementTree as ET
from map.mapdata import MapData
from common.rect import Rect
from common.config import Config
from map.validation import MapFileValidator, check_outlines_overlapping
from map.parser import MapDataParser
from map.tileset import TilesetsDatabase

class MapsLoader:
    def __init__(self, config: Config) -> None:
        self.config = config
        self.tilesetsDatabase = TilesetsDatabase()
        self.maps_outlines: list[Rect] = []
        self.min_elevation_index = None
        self.max_elevation_index = None


    def get_mapfile_root(self, filename) -> tuple[str, ET.Element]:
        map_path = os.path.join(self.config.maps_dirpath, filename)

        """ Load map data from file """
        tree = ET.parse(map_path)
        if tree is None:
            print(f"Error loading map: {map_path}")
            return None
        
        return map_path, tree.getroot()


    def preflight(self, filename: str) -> bool:
        map_path, root = self.get_mapfile_root(filename)
        
        """ Validate map data xml - check all necessary fields are present """
        validator = MapFileValidator(self.config)
        if not validator.validate(map_path, root):
            print(f"Error validating map: {map_path}")
            return False
        elif self.config.verbose:
            print(f"Map validated: {map_path}")

        """ Parse map data xml """
        parser = MapDataParser(self.config)

        tileset_dicts = parser.parseTilesetsData(root)
        for tileset_dict in tileset_dicts:
            self.tilesetsDatabase.add(tileset_dict)

        """ Parse map cuboid """
        """ Add outlines, find min/max elevation index """
        map_cuboid_dict = parser.parseMapCuboid(root)
        outline_rect = map_cuboid_dict["outline_rect"]
        self.maps_outlines.append(outline_rect)

        if self.min_elevation_index is None:
            self.min_elevation_index = map_cuboid_dict["min_elevation_idx"]
        elif map_cuboid_dict["min_elevation_idx"] < self.min_elevation_index:
            self.min_elevation_index = map_cuboid_dict["min_elevation_idx"]

        if self.max_elevation_index is None:
            self.max_elevation_index = map_cuboid_dict["max_elevation_idx"]
        elif map_cuboid_dict["max_elevation_idx"] > self.max_elevation_index:
            self.max_elevation_index = map_cuboid_dict["max_elevation_idx"]

        return True


    def loadMapData(self, filename: str) -> MapData:
        map_path, root = self.get_mapfile_root(filename)

        """ Parse map data xml """
        parser = MapDataParser(self.config)
        map_data = parser.parseMapData(map_path, root, self.tilesetsDatabase)
        if map_data is None:
            print(f"Error parsing map: {map_path}")
            return None
        
        if self.config.verbose:
            print(f"Map loaded succesfully: {map_path}", end="\n\n")

        return map_data
    

    def validateAndloadAllMapsData(self) -> list[MapData]:
        maps_data = []

        if not (os.path.exists(self.config.maps_dirpath) 
                and os.path.isdir(self.config.maps_dirpath)):
            return None
        
        valid_filenames = [filename for filename in os.listdir(self.config.maps_dirpath) if filename.endswith(".tmx")]
        
        """ Preflight - all tilesets load, collision check """
        for filename in valid_filenames:
            if not self.preflight(filename):
                return None
        # tilesets are stored in self.tilesetsDatabase
        # outlines are stores in self.maps_outlines
            
        if self.config.verbose:
            print(f"All maps [{len(valid_filenames)}] validated", end="\n\n")
            
        """ Check for collision between maps """
        if check_outlines_overlapping(self.maps_outlines):
            print(f"Error: overlapping between [{len(self.maps_outlines)}] maps")
            return None

        if self.config.verbose:
            print(f"No overlappings between [{len(self.maps_outlines)}] maps")

        """ Organize tilesets database """
        self.tilesetsDatabase.organize()
        # tilesetsDatabase is locked now
        
        if self.config.verbose:
            print(f"Tilesets [{len(self.tilesetsDatabase)}] organized")
            print(self.tilesetsDatabase)

        if self.config.verbose:
            print("Loading maps from: ", self.config.maps_dirpath, end="\n\n")

        """ Load all maps data """
        for filename in valid_filenames:
            map_data = self.loadMapData(filename)

            if map_data is None:
                """ Error loading map data """
                return None

            maps_data.append(map_data)
        return maps_data

