from map.elevationdata import Elevation
from common.rect import Rect
from typing import Dict

class MapData:
    def __init__(self, outline_rect: Rect, min_elevation_idx: float, max_elevation_idx: float, elevations_count: float, filepath: str):
        self.outline = outline_rect
        self.min_elevation_idx = min_elevation_idx
        self.max_elevation_idx = max_elevation_idx
        self.elevations_count = elevations_count
        self.filepath = filepath
        self.tilesets_ids: list[int] = []
        self.elevations_map: Dict[int, Elevation] = {}
        self.locked = False


    def set_elevations(self, elevations: Dict[int, Elevation]) -> None:
        if self.locked:
            raise Exception("MapData is locked")
        if len(elevations.keys()) != self.elevations_count:
            raise Exception(f"Expected {self.elevations_count} elevations, got {len(elevations)}")
        if self.min_elevation_idx != list(elevations.keys())[0]:
            raise Exception(f"Expected min elevation index {self.min_elevation_idx}, got {elevations[0].index}")
        if self.max_elevation_idx != list(elevations.keys())[-1]:
            raise Exception(f"Expected max elevation index {self.max_elevation_idx}, got {elevations[-1].index}")
    
        """ Sort just in case """
        sorted_elevations = dict(sorted(elevations.items(), key=lambda item: item[0]))
        self.elevations_map = sorted_elevations


    def add_tilesets_ids(self, tilesets_ids: list[int]) -> None:
        if self.locked:
            raise Exception("MapData is locked")
        self.tilesets_ids.extend(tilesets_ids)


    def validate(self) -> bool:
        if self.locked:
            raise Exception("MapData is locked")

        if len(self.elevations_map.keys()) != self.elevations_count:
            return False

        for _, elevation in self.elevations_map.items():
            if not elevation.validate():
                return False
            
        self.locked = True
        return True
    
    
    def get_gid_at(self, global_x: int, global_y: int, elevation_idx: int, layer_idx: int) -> int:
        local_x = global_x - self.outline.x
        local_y = global_y - self.outline.y

        if local_x < 0 or local_x >= self.outline.width:
            raise Exception(f"X={global_x} -> {local_x} not inside map")

        if local_y < 0 or local_y >= self.outline.height:
            raise Exception(f"Y={global_y} -> {local_y} not inside map")

        if elevation_idx not in self.elevations_map.keys():
            raise Exception(f"Not found elevation {elevation_idx}")
        
        elevation = self.elevations_map[elevation_idx]

        gid = elevation.get_gid_at(local_x, local_y, layer_idx)
        if gid is None:
            raise Exception(f"Not found layer {layer_idx}")

        return gid

    
    def __str__(self) -> str:
        string = f"MapData: xywh={self.outline}, elevation_idx={self.min_elevation_idx}-{self.max_elevation_idx}, tilesets_ids={self.tilesets_ids}, elevations[{self.elevations_count}]:\n"
        for elevation_idx, elevation in self.elevations_map.items():
            string += f" * Elevation[{elevation_idx}], has_floor={elevation.floor_layer is not None}, has_details={elevation.details_layer is not None}, objects_layers_count={len(elevation.objects_layers)}\n"
            all_layers = elevation.get_all_layers()
            for layer in all_layers:
                string += f"   - {layer}\n"
        return string
