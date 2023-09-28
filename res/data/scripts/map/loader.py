import os
import xml.etree.ElementTree as ET
from map.mapdata import MapData
from common.rect import Rect
from common.config import Config

class MapsLoader:
    def __init__(self, config: Config) -> None:
        self.config = config

    def load(self, filename) -> MapData:
        map_path = os.path.join(self.config.maps_dirpath, filename)
        if self.config.verbose:
            print(f"Loading map: {map_path}")

        """ Load map data from file """

        """ Validate map data xml - check all necessary fields are present """

        """ Parse map data xml """

        return None

    def loadAll(self) -> list[MapData]:
        maps_data = []
        if self.config.verbose:
            print("Loading maps from: ", self.config.maps_dirpath)

        """ Load all maps data """
        for filename in os.listdir(self.config.maps_dirpath):
            if filename.endswith(".tmx"):
                map_data = self.load(filename)

                if map_data is None:
                    """ Error loading map data """
                    return None

                maps_data.append(map_data)
        return maps_data

