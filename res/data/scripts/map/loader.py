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
        self.maps_outlines = [Rect]


    def get_mapfile_root(self, filename) -> tuple[str, ET.Element]:
        map_path = os.path.join(self.config.maps_dirpath, filename)
        
        if self.config.verbose:
            print(f"Loading map: {map_path}")

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

        return True


    def load(self, filename: str) -> MapData:

        # """ Parse map data xml """
        # parser = MapDataParser(self.config)
        # tilesets_data, map_data = parser.parse(map_path, root)
        # if map_data is None:
        #     print(f"Error parsing map: {map_path}")
        #     return None
        # elif self.config.verbose:
        #     print(f"Map parsed: {map_path}")

        # return map_data
        return None
    

    def loadAll(self) -> list[MapData]:
        maps_data = []
        if self.config.verbose:
            print("Loading maps from: ", self.config.maps_dirpath)

        if not (os.path.exists(self.config.maps_dirpath) 
                and os.path.isdir(self.config.maps_dirpath)):
            return None
        
        valid_filenames = [filename for filename in os.listdir(self.config.maps_dirpath) if filename.endswith(".tmx")]
        
        """ Preflight - all tilesets load, collision check """
        for filename in valid_filenames:
            if not self.preflight(filename):
                return None
            
        """ Check for collision between maps """
        if check_outlines_overlapping(self.maps_outlines):
            print("Error: overlapping between maps")
            return None

        if self.config.verbose:
            print("No overlappings between maps")

        """ Organize tilesets database """
        self.tilesetsDatabase.organize()
        
        if self.config.verbose:
            print("Tilesets organized")
            print(self.tilesetsDatabase)

        """ Load all maps data """
        for filename in valid_filenames:
            map_data = self.load(filename)

            if map_data is None:
                """ Error loading map data """
                return None

            maps_data.append(map_data)
        return maps_data

