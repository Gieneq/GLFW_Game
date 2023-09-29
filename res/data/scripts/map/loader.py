import os
import xml.etree.ElementTree as ET
from map.mapdata import MapData
from common.rect import Rect
from common.config import Config
from map.validation import MapFileValidator

class MapsLoader:
    def __init__(self, config: Config) -> None:
        self.config = config

    def load(self, filename) -> MapData:
        map_path = os.path.join(self.config.maps_dirpath, filename)
        
        if self.config.verbose:
            print(f"Loading map: {map_path}")

        """ Load map data from file """
        tree = ET.parse(map_path)
        if tree is None:
            print(f"Error loading map: {map_path}")
            return None
        
        root = tree.getroot()

        """ Validate map data xml - check all necessary fields are present """
        validator = MapFileValidator(self.config)
        if not validator.validate(map_path, root):
            print(f"Error validating map: {map_path}")
            return None
        elif self.config.verbose:
            print(f"Map validated: {map_path}")

        """ Parse map data xml """

        print("todo map")
        return None

    def loadAll(self) -> list[MapData]:
        maps_data = []
        if self.config.verbose:
            print("Loading maps from: ", self.config.maps_dirpath)

        if not (os.path.exists(self.config.maps_dirpath) 
                and os.path.isdir(self.config.maps_dirpath)):
            return None

        """ Load all maps data """
        for filename in os.listdir(self.config.maps_dirpath):
            if filename.endswith(".tmx"):
                map_data = self.load(filename)

                if map_data is None:
                    """ Error loading map data """
                    return None

                maps_data.append(map_data)
        return maps_data

