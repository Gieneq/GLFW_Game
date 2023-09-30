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
        string = f"MapData(filepath={self.filepath}, outline={self.outline}, min_elevation_idx={self.min_elevation_idx}, max_elevation_idx={self.max_elevation_idx}), elevations_count={self.elevations_count} tilesets_ids={self.tilesets_ids}, elevations:\n"
        for _, elevation in self.elevations_map.items():
            string += str(elevation)

        return string


# class MapData(Rect):
#     def __init__(self, data):
#         super().__init__(data["global_x"], data["global_y"], data["width"], data["height"])
#         self.z = data["global_z"]
#         self.depth = data["depth"]

#         self.filepath = data["filepath"]

#         """
#         Elevations structure:
#         elevations: list of elevation: 
#           list of layers: 
#             list of GID of len = chunk_width * chunk_height
#         access elevation: elevations[0]
#         access layer: elevations[0][1]
#         """
#         self.elevations = []

#     def __str__(self):
#         string = f"MapData(filepath={self.filepath}, x={self.x}, y={self.y}, z={self.z}, width={self.width}, height={self.height}, depth={self.depth}), elevations:\n"
#         for elevation_idx, elevation in enumerate(self.elevations):
#             string += f"* elevation {elevation_idx}:\n"
#             for layer_idx, layer in enumerate(elevation):
#                 string += f"  - elevation {layer_idx}, indices: {len(layer)}\n"

#         return string

#     def get_gid_at(self, elevation_idx, layer_idx, global_x, global_y):
#         local_x = global_x - self.x
#         local_y = global_y - self.y

#         if local_x < 0 or local_x >= self.width:
#             raise PositionError(f"X={global_x} -> {local_x} not inside map")

#         if local_y < 0 or local_y >= self.height:
#             raise PositionError(f"Y={global_y} -> {local_y} not inside map")

#         if elevation_idx < 0 or len(self.elevations) <= elevation_idx:
#             raise ElevationError(f"Not found elevation {elevation_idx}")

#         layers = self.elevations[elevation_idx]

#         if 0 > layer_idx or layer_idx >= len(layers):
#             raise LayerError(f"Not found layer {layer_idx}")

#         gids = layers[layer_idx]

#         value_idx = local_x + local_y * self.width

#         if 0 > value_idx or value_idx >= len(gids):
#             raise PositionError(f"Y={global_y} not inside map")

#         try:
#             return gids[value_idx]
#         except IndexError as e:
#             print(f"index {value_idx} not found in gids of len={len(gids)}, local=({local_x}, {local_x})")
#             raise e

#         return None


#     @staticmethod
#     def map_datas_have_overlapping(map_datas):
#         for map_idx, map_data in enumerate(map_datas):
#             for other_idx in range(map_idx + 1, len(map_datas)):
#                 other_data = map_datas[other_idx]
#                 if map_data.intersect(other_data):
#                     return True
#         return False

#     @staticmethod
#     def map_datas_get_bounding_rect(map_datas):

#         if len(map_datas) == 0:
#             return None

#         if len(map_datas) == 1:
#             return Rect.create_from(map_datas)

#         """
#         Accumulate bounding_rect
#         """
#         min_left = map_datas[0].left()
#         max_right = map_datas[0].right()
#         min_top = map_datas[0].top()
#         max_bottom = map_datas[0].bottom()

#         for map_data in map_datas:
#             if map_data.left() < min_left:
#                 min_left = map_data.left()
#             if map_data.right() > max_right:
#                 max_right = map_data.right()

#             if map_data.top() < min_top:
#                 min_top = map_data.top()
#             if map_data.bottom() > max_bottom:
#                 max_bottom = map_data.bottom()

#         return Rect(min_left, min_top, max_right - min_left, max_bottom - min_top)