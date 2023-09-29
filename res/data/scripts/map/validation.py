from common.config import Config
import xml.etree.ElementTree as ET
import os
from common.rect import Rect

class MapFileValidator:
    def __init__(self, config: Config):
        self.config = config

    def validate(self, file_path: str, root: ET.Element) -> bool:
        data = {}
        """ Requires properties """
        properties = root.find(".//properties")
        if properties is None:
            print(f"Missing properties: {file_path}")
            return False
        
        """ Requires global X, Y, Z """
        data["global_x"] = int(properties.find(".//property[@name='global_X']").attrib['value'])
        data["global_y"] = int(properties.find(".//property[@name='global_Y']").attrib['value'])
        data["global_z"] = int(properties.find(".//property[@name='global_Z']").attrib['value'])

        if data["global_x"] is None or data["global_y"] is None or data["global_z"] is None:
            print(f"Missing global_x, global_y or global_z: {file_path}")
            return False

        """ Tile width/height should match preset """
        data["tile_width"] = int(root.attrib['tilewidth'])
        data["tile_height"] = int(root.attrib['tileheight'])
        
        if data["tile_width"] != self.config.tile_width or data["tile_height"] != self.config.tile_height:
            print(f"Tile width/height mismatch: {file_path}")
            return False
        
        """ Check if tileset files exists """
        tilesets = root.findall(".//tileset")
        for tileset in tilesets:
            tileset_dir = tileset.attrib['source']
            tileset_dirpath = os.path.join(self.config.maps_dirpath, tileset_dir)
            if not os.path.exists(tileset_dirpath):
                print(f"Missing tileset: {tileset_dirpath}")
                return False
        
        """ Check if has any group """
        groups = root.findall(".//group")
        if len(groups) == 0:
            print(f"No group in map, seems corrupted: {file_path}")
            return False

        return True
    
    
def check_outlines_overlapping(outlines: list[Rect]) -> bool:
    if len(outlines) < 2:
        return False
    
    for idx, outline in enumerate(outlines):
        for other_idx in range(idx + 1, len(outlines)):
            other_outline = outlines[other_idx]
            if outline.is_intersecting(other_outline):
                return True
    return False