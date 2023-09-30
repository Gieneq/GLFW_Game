from common.rect import Rect
from map.elevationdata import Elevation
from typing import Dict

class Chunk:
    def __init__(self, x:float, y: float, width: float, height:float, min_elevation_idx: int, max_elevation_idx: int, elevations_count: int) -> None:
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.min_elevation_idx = min_elevation_idx
        self.max_elevation_idx = max_elevation_idx
        self.elevations_count = elevations_count
        self.elevations_map: Dict[int, Elevation] = {}
        self.tilesets_ids: list[int] = []
        self.locked = False
    

    def set_elevations(self, elevations: Dict[int, Elevation]):
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
        self.locked = True
        return len(self.tilesets_ids) > 0 and len(self.elevations_map.keys()) > 0