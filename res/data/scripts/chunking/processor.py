from common.config import Config
from map.mapdata import MapData
from map.elevationdata import Elevation, Layer
from common.rect import Rect
from .chunk import Chunk
from map.tileset import TilesetsDatabase
from typing import Dict

class ChunkingProcessor:
    def __init__(self, config: Config, tilesets_database: TilesetsDatabase) -> None:
        self.config = config
        self.tilesets_database = tilesets_database
        
        self.tileset_of_default_tile = tilesets_database.get_tileset_with_gid(self.config.default_tile_gid)
        if self.tileset_of_default_tile is None:
            raise Exception("Tileset of default tile is None")
        
        self.tileset_id_of_default_tile = tilesets_database.get_id_of_tileset(self.tileset_of_default_tile)
        if self.tileset_id_of_default_tile is None:
            raise Exception("Tileset ID of default tile is None")
        
        self.bounding_rect_not_aligned = None
        self.bounding_rect_aligned = None
        self.chunks_x = 0
        self.chunks_y = 0
        

    @property
    def chunks_count(self) -> int:
        return self.chunks_x * self.chunks_y

    def get_bounding_rect_around_maps(self, map_datas: list[MapData]) -> Rect:
        if len(map_datas) == 0:
            return None

        if len(map_datas) == 1:
            return Rect.create_from(map_datas[0].outline)

        """
        Accumulate bounding_rect
        """
        min_left = map_datas[0].outline.left()
        max_right = map_datas[0].outline.right()
        min_top = map_datas[0].outline.top()
        max_bottom = map_datas[0].outline.bottom()

        for map_data in map_datas:
            if map_data.outline.left() < min_left:
                min_left = map_data.outline.left()
            if map_data.outline.right() > max_right:
                max_right = map_data.outline.right()

            if map_data.outline.top() < min_top:
                min_top = map_data.outline.top()
            if map_data.outline.bottom() > max_bottom:
                max_bottom = map_data.outline.bottom()

        return Rect(min_left, min_top, max_right - min_left, max_bottom - min_top)


    def splitMapsIntoChunks(self, map_data_list: list[MapData]) -> list[Chunk]:
        if self.config.verbose:
            print("-------------------------------------------------------------")
            print("Splitting maps into chunks...")

        chunks = []

        """ Create grid aligned bounding rect """
        self.bounding_rect_not_aligned = self.get_bounding_rect_around_maps(map_data_list)
        self.bounding_rect_aligned = self.bounding_rect_not_aligned.get_align_to(self.config.chunk_width, self.config.chunk_height)
        self.chunks_x = int(self.bounding_rect_aligned.width / self.config.chunk_width)
        self.chunks_y = int(self.bounding_rect_aligned.height / self.config.chunk_height)

        if self.config.verbose:
            print(f"Bounding rect: {self.bounding_rect_not_aligned}")
            print(f"Bounding rect aligned: {self.bounding_rect_aligned}")
            print(f"Chunks: {self.chunks_x} x {self.chunks_y} = {self.chunks_count}")

        """
        Creation & visualization of chunks
        """
        for chunk_y in range(self.chunks_y):
            for chunk_x in range(self.chunks_x):
                window_chunk = Rect(
                    self.bounding_rect_aligned.x + chunk_x * self.config.chunk_width,
                    self.bounding_rect_aligned.y + chunk_y * self.config.chunk_height,
                    self.config.chunk_width,
                    self.config.chunk_height
                )

                # Check if window has collision with any map
                intersecting_maps = []
                for map_data in map_data_list:
                    if window_chunk.is_intersecting(map_data.outline):
                        intersecting_maps.append(map_data)

                chunk = self.buildChunk(window_chunk, intersecting_maps)
                if chunk is None:
                    print("Error: Chunk is None")
                    return None
                
                chunks.append(chunk)

                if len(intersecting_maps) == 0:
                    print(".", end="")
                else:
                    print(f"{len(intersecting_maps)}", end="")
            print("")
        return chunks
    

    def buildChunk(self, window_chunk: Rect, intersecting_maps: list[MapData]) -> Chunk:
        min_elevation_idx = 0 if len(intersecting_maps) == 0 else min((map_data.min_elevation_idx for map_data in intersecting_maps))
        max_elevation_idx = 0 if len(intersecting_maps) == 0 else max((map_data.max_elevation_idx for map_data in intersecting_maps))
        elevations_count = max_elevation_idx - min_elevation_idx + 1
        if min_elevation_idx > max_elevation_idx:
            raise Exception("min_elevation_idx > max_elevation_idx")
        

        chunk = Chunk(
            x=window_chunk.x, 
            y=window_chunk.y, 
            width=window_chunk.width, 
            height=window_chunk.height,
            min_elevation_idx=min_elevation_idx,
            max_elevation_idx=max_elevation_idx,
            elevations_count=elevations_count
        )

        elevations: Dict[int, Elevation] = {}

        if len(intersecting_maps) == 0:
            """ Create elevations with default tile floor-layer """
            elevation = Elevation(index=0)

            layer = Layer(
                name="floor_common",
                width=self.config.chunk_width,
                height=self.config.chunk_height,
                void_gid=self.config.void_tile_gid
            )

            layer.set_data([self.config.default_tile_gid] * self.config.chunk_width * self.config.chunk_height, self.config)

            elevation.add_layer(layer)
            elevations[0] = elevation

            chunk.add_tilesets_ids([self.tileset_id_of_default_tile])
        else:
            """ Create elevations from interseting maps """
            bag_of_tilesets_ids: int = []
            for map_data in intersecting_maps:
                bag_of_tilesets_ids.extend(map_data.tilesets_ids)

            common_tileset_ids = list(set(bag_of_tilesets_ids))
            common_tileset_ids.sort()
            chunk.add_tilesets_ids(common_tileset_ids)

            for elevation_idx in range(min_elevation_idx, max_elevation_idx + 1):
                combined_elevation = Elevation(index=elevation_idx)

                # : list[tuple[tuple[int, int], Elevation]
                found_elevations = []

                for map_data in intersecting_maps:
                    if elevation_idx in map_data.elevations_map.keys():
                        found_world_position = (map_data.outline.x, map_data.outline.y)
                        found_elevations.append((found_world_position, map_data.elevations_map[elevation_idx]))

                """ Populate elevation layers with combined layers from intersecting elevations """

                has_floor_layer = any([elevation.floor_layer is not None for _, elevation in found_elevations])
                has_details_layer = any([elevation.details_layer is not None for _, elevation in found_elevations])
                
                layers_count = max([elevation.get_layers_count() for _, elevation in found_elevations])
                for layer_idx in range(layers_count):
                    layer_name = "objects"
                    if layer_idx == 0:
                        if not has_floor_layer:
                            combined_elevation.floor_layer = None
                            continue
                        layer_name = "floor"

                    elif layer_idx == 1:
                        if not has_details_layer:
                            combined_elevation.details_layer = None
                            continue
                        layer_name = "details"

                    combined_layer = Layer(
                        name=layer_name,
                        width=window_chunk.width,
                        height=window_chunk.height,
                        void_gid=self.config.void_tile_gid
                    )

                    found_layers: list[Layer] = []
                    for world_pos, elevation in found_elevations:
                        equal_level_layer = elevation.get_all_layers()[layer_idx]
                        if equal_level_layer is not None:
                            found_layers.append((world_pos, equal_level_layer))

                    """ Create layer data """
                    combined_layer.set_data([self.config.void_tile_gid] * window_chunk.width * window_chunk.height, self.config)

                    for iy in range(window_chunk.height):
                        for ix in range(window_chunk.width):
                            world_x = window_chunk.x + ix
                            world_y = window_chunk.y + iy
                            for found_layer_position, found_layer in found_layers:
                                found_gid = found_layer.get_gid_at(
                                    x=-found_layer_position[0] + world_x,
                                    y=-found_layer_position[1] + world_y
                                )
                                if found_gid is not None and found_gid != self.config.void_tile_gid:
                                    if combined_layer.get_gid_at(ix, iy) != self.config.void_tile_gid:
                                        raise Exception(f"combined_layer.get_git_at(ix, iy) not null and found_gid also: {combined_layer.get_gid_at(ix, iy)} != {found_gid}")
                                    combined_layer.set_gid_at(ix, iy, found_gid)

                    """ Prevent creating void layers """
                    if not combined_layer.is_void_filled():
                        combined_elevation.add_layer(combined_layer)

                #todo prevent empty elevations
                elevations[elevation_idx] = combined_elevation

        """ Finally add elevations to chunk """
        chunk.set_elevations(elevations)

        if not chunk.validate():
            return None
        
        return chunk
        
