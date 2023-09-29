import xml.etree.ElementTree as ET
from common.config import Config
from map.mapdata import MapData
from common.rect import Rect
import os
from map.elevationdata import Elevation, Layer

class MapDataParser:
    def __init__(self, config: Config) -> None:
        self.config = config

    def parse(self, file_path: str, root: ET.Element) -> MapData:
        """ Parse map data xml """
        general_data = self.parseGeneralData(root)
        tilesets_data = self.parseTilesetsData(root)

        """ Parse elevations """
        elevations = self.parseElevations(root)

        """ Build map data """

        return None
    
    def parseGeneralData(self, root: ET.Element) -> dict:

        """ Retrive properties - was validated """
        properties = root.find("properties")

        """ Create flatten outline in global space """
        outline_rect = Rect(
            int(properties.find("property[@name='global_X']").attrib["value"]),
            int(properties.find("property[@name='global_Y']").attrib["value"]), 
            int(root.attrib["width"]),
            int(root.attrib["height"])
        )

        """ Get Z axis values """
        global_z = int(properties.find("property[@name='global_Z']").attrib["value"])
        depth = len(root.findall(".//group"))

        return {
            "outline_rect": outline_rect,
            "global_z": global_z,
            "depth": depth
        }
    
    def parseTilesetsData(self, root: ET.Element) -> list[dict]:
        tilesets = root.findall("tileset")
        tilesets_data = []

        for tileset in tilesets:
            tileset_path =  os.path.join(self.config.maps_dirpath, tileset.attrib["source"])
            tileset_path = os.path.normpath(tileset_path)

            tileset_data = {
                "first_gid": int(tileset.attrib["firstgid"]),
                "tileset_path": tileset_path,
            }

            tilesets_data.append(tileset_data)

        return tilesets_data
    
    def parseElevations(self, root: ET.Element) -> list[Elevation]:
        return []