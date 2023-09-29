import xml.etree.ElementTree as ET
from common.config import Config
from map.mapdata import MapData
from common.rect import Rect
import os
from map.elevationdata import Elevation, Layer
from map.tileset import TilesetsDatabase

class MapDataParser:
    def __init__(self, config: Config) -> None:
        self.config = config

    def parseMapData(self, file_path: str, root: ET.Element, 
                     tilesetDatabase: TilesetsDatabase) -> MapData:
        """ Parse map data xml """
        map_cuboid_dict = self.parseMapCuboid(root)
        tilesets_ids = tilesetDatabase.get_ids_of_tilesets(self.parseTilesetsData(root))

        """ Parse elevations """
        elevations = self.parseElevations(root)

        """ Build map data """
        map_data = MapData(
            outline_rect=map_cuboid_dict["outline_rect"],
            global_z=map_cuboid_dict["global_z"],
            depth=map_cuboid_dict["depth"],
            filepath=file_path
        )

        map_data.add_elevations(elevations)
            
        """ Validate map data """
        if not map_data.validate():
            raise Exception(f"Error validating map: {file_path}")

        return map_data
    
    def parseMapCuboid(self, root: ET.Element) -> dict:

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
        elevation_node_name = "group"
        elevations: list[Elevation] = []

        for elevation_idx, elevation_node in enumerate(root.findall(f".//group")):
            elevation = self.parseElevation(elevation_idx, elevation_node)
            if elevation:
                elevations.append(elevation)

        return elevations
    
    def parseElevation(self, elevation_idx: int, elevation_node: ET.Element) -> Elevation:
        layer_node_name = "layer"
        layer_data_node_name = "data"

        elevation = Elevation(elevation_idx)
        for layer_node in elevation_node.findall(f".//{layer_node_name}"):
            layer_data_node = layer_node.find(f".//{layer_data_node_name}")

            if layer_data_node is None:
                raise Exception(f"Missing {layer_data_node_name} node")

            layer = self.parseLayer(layer_node, layer_data_node)
            if layer is None:
                raise Exception(f"Error parsing layer")
            
            elevation.add_layer(layer)

        # Layers are validated upon adding

        return elevation
    

    def parseLayer(self, layer_node: ET.Element, layer_data_node: ET.Element) -> Layer:
        layer = Layer(
            name=layer_node.attrib["name"],
            width=int(layer_node.attrib["width"]),
            height=int(layer_node.attrib["height"])
        )

        data = layer_data_node.text.strip()
        data = data.split(",")

        if len(data) != layer.width * layer.height:
            raise Exception(f"Invalid data length: {len(data)}")

        layer.set_data([int(gid) for gid in data], self.config)
        return layer