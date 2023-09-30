import xml.etree.ElementTree as ET
from common.config import Config
from map.mapdata import MapData
from common.rect import Rect
import os
from map.elevationdata import Elevation, Layer, calculate_z_index
from map.tileset import TilesetsDatabase
from typing import Dict

class MapDataParser:
    def __init__(self, config: Config) -> None:
        self.config = config


    def parseMapData(self, file_path: str, root: ET.Element, 
                     tilesetDatabase: TilesetsDatabase) -> MapData:
        """ Parse map data xml """
        map_cuboid_dict = self.parseMapCuboid(root)
        tilesets_ids = tilesetDatabase.get_ids_of_tilesets(self.parseTilesetsData(root))

        """ Parse elevations """
        elevations_dict = self.parseElevations(root)

        """ Build map data """
        map_data = MapData(
            outline_rect=map_cuboid_dict["outline_rect"],
            min_elevation_idx=map_cuboid_dict["min_elevation_idx"],
            max_elevation_idx=map_cuboid_dict["max_elevation_idx"],
            elevations_count=map_cuboid_dict["elevations_count"],
            filepath=file_path
        )

        map_data.set_elevations(elevations_dict)
        map_data.add_tilesets_ids(tilesets_ids)
            
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
        elevations_nodes = root.findall(".//group")
        
        """ After validation there iss at least 1 group (elevation) and are sorted from bottom to top """
        elevations_count = len(elevations_nodes)
        min_elevation_node = elevations_nodes[0]

        """ Get elevation Z index """
        min_elevation_idx = 0 if "offsety" not in min_elevation_node.attrib else calculate_z_index(int(min_elevation_node.attrib["offsety"]), self.config.tile_height)

        return {
            "outline_rect": outline_rect,
            "min_elevation_idx": min_elevation_idx,
            "max_elevation_idx": min_elevation_idx + elevations_count - 1,
            "elevations_count": elevations_count
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
    

    def parseElevations(self, root: ET.Element) -> Dict[int, Elevation]:
        elevation_node_name = "group"
        elevations_dict: Dict[int, Elevation] = {}

        for _, elevation_node in enumerate(root.findall(f".//{elevation_node_name}")):
            # warning - elevation index is not just order
            elevation_idx, elevation = self.parseElevation(elevation_node)
            if elevation_idx is not None and elevation is not None:
                elevations_dict[elevation_idx] = elevation

        return elevations_dict
    

    def parseElevation(self, elevation_node: ET.Element) -> tuple[int, Elevation]:
        elevation_idx = 0 if "offsety" not in elevation_node.attrib else calculate_z_index(int(elevation_node.attrib["offsety"]), self.config.tile_height)
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

        return elevation_idx, elevation
    

    def parseLayer(self, layer_node: ET.Element, layer_data_node: ET.Element) -> Layer:
        layer = Layer(
            name=layer_node.attrib["name"],
            width=int(layer_node.attrib["width"]),
            height=int(layer_node.attrib["height"]),
            void_gid=self.config.void_tile_gid
        )

        data = layer_data_node.text.strip()
        data = data.split(",")

        if len(data) != layer.width * layer.height:
            raise Exception(f"Invalid data length: {len(data)}")

        layer.set_data([int(gid) for gid in data], self.config)
        return layer