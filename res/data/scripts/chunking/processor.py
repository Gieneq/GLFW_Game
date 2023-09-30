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
        self.tileset_of_default_tile = tilesets_database.get_tileset_by_id(self.config.default_tile_gid)
        if self.tileset_of_default_tile is None:
            raise Exception("Tileset of default tile is None")

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

            chunk.add_tilesets_ids([self.tileset_of_default_tile])
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

                found_elevations: list[Elevation] = []

                for map_data in intersecting_maps:
                    if elevation_idx in map_data.elevations_map.keys():
                        found_elevations.append(map_data.elevations_map[elevation_idx])

                """ Populate elevation layers with combined layers from intersecting elevations """

                objects_layers_count = max([len(elevation.objects_layers) for elevation in found_elevations])
                has_floor_layer = any([elevation.floor_layer is not None for elevation in found_elevations])
                has_details_layer = any([elevation.details_layer is not None for elevation in found_elevations])

                layers_count = objects_layers_count + (1 if has_floor_layer else 0) + (1 if has_details_layer else 0)

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

                    """ Create layer data """
                    combined_layer.set_data([self.config.void_tile_gid] * window_chunk.width * window_chunk.height, self.config)

                    for iy in range(window_chunk.height):
                        for ix in range(window_chunk.width):
                            world_space_x = ix + window_chunk.x
                            world_space_y = iy + window_chunk.y

                            for map_data in intersecting_maps:
                                try:
                                    gid = map_data.get_gid_at(world_space_x, world_space_y, elevation_idx, layer_idx)
                                    if combined_layer.get_git_at(ix, iy) != self.config.void_tile_gid:
                                        raise Exception(f"Building chunk failed - non void tile overlapped somehow")
                                    combined_layer.set_gid_at(ix, iy, gid)
                                except Exception:
                                    pass

                    combined_elevation.add_layer(combined_layer)


                """ Check layers count """
                if combined_elevation.get_layers_count() != layers_count:
                    raise Exception("combined_elevation.get_layers_count() != layers_count")
                elevations[elevation_idx] = combined_elevation

        """ Finally add elevations to chunk """
        chunk.set_elevations(elevations)

        if not chunk.validate():
            return None
        
        return chunk
        




            #     chunk_content = self._build_chunk(window_chunk, chunk_z_min, intersecting_maps)
            #     if chunk_content is None:
            #         return False

            #     self.world_data["tiles_count"] += chunk_content["tiles_count"]

            #     chunk_filename = f"chunk_{chunk_content['x']}_{chunk_content['y']}.json"
            #     chunk_filepath = os.path.join(self.output_data_dir, chunk_filename)
            #     chunk_filepath = self._unify_path(chunk_filepath)

            #     if not self._save_chunk_data(chunk_content, chunk_filepath):
            #         return False

            #     chunk_d = len(chunk_content["elevations"])
            #     chunk_z_max = chunk_z_min + chunk_d

            #     """ Add to world data for caching """
            #     if chunk_z_min < self.world_data["bounding_z_min"]:
            #         self.world_data["bounding_z_min"] = chunk_z_min

            #     if chunk_z_max > self.world_data["bounding_z_max"]:
            #         self.world_data["bounding_z_max"] = chunk_z_max

            #     self.world_data["tileset_ids_set"] = self.world_data["tileset_ids_set"].union(chunk_content["tilesets"])
            #     self.world_data["chunks_data"].append({
            #         "x": window_chunk.x,
            #         "y": window_chunk.y,
            #         "z": chunk_z_min,
            #         "d": chunk_d,
            #         "file": chunk_filepath,
            #         "tiles_count": chunk_content["tiles_count"],
            #         "tilesets": chunk_content["tilesets"],
            #     })






            
    # def _build_chunk(self, window_chunk, chunk_z_min, intersecting_maps):
    #     chunk_data = {
    #         "x": window_chunk.x,
    #         "y": window_chunk.y,
    #         "z": chunk_z_min,
    #         "tilesets": None,
    #         "tiles_count": None,
    #         "elevations": None,
    #     }

    #     final_elevations = []
    #     if len(intersecting_maps) == 0:
    #         """
    #         Empty chunk - create one elevation with one layer 
    #         filled with default tiles global ID.
    #         """
    #         layer = [self.default_tile_gid] * self.chunk_width * self.chunk_height
    #         elevation = [layer]
    #         final_elevations.append(elevation)

    #     else:
    #         """
    #         Find shape of chunk - how many elevations and layers each.
    #         Fill them all with void values.
    #         """

    #         elevations_count = max([len(map_data.elevations) for map_data in intersecting_maps])
    #         for elevation_idx in range(elevations_count):
    #             found_elevations = []

    #             for map_data in intersecting_maps:
    #                 try:
    #                     elevation = map_data.elevations[elevation_idx]
    #                     found_elevations.append(elevation)
    #                 except IndexError:
    #                     pass

    #             combined_elevation = []

    #             layers_count = max([len(elevation) for elevation in found_elevations])
    #             for layer_idx in range(layers_count):
    #                 combined_layer = [self.void_tile_gid] * self.chunk_width * self.chunk_height
    #                 for iy in range(window_chunk.height):
    #                     for ix in range(window_chunk.width):
    #                         world_space_x = ix + window_chunk.x
    #                         world_space_y = iy + window_chunk.y

    #                         for map_data in intersecting_maps:
    #                             try:
    #                                 gid = map_data.get_gid_at(elevation_idx, layer_idx, world_space_x, world_space_y)
    #                                 if combined_layer[ix + iy * window_chunk.width] != self.void_tile_gid:
    #                                     raise BuildingError(f"Building chunk failed - non void tile overlapped somehow")

    #                                 combined_layer[ix + iy * window_chunk.width] = gid
    #                             except ElevationError as e_err:
    #                                 pass
    #                             except LayerError as l_err:
    #                                 pass
    #                             except PositionError as p_err:
    #                                 pass

    #                 combined_elevation.append(combined_layer)
    #             final_elevations.append(combined_elevation)

    #     chunk_data["elevations"] = final_elevations

    #     """
    #     Resources info
    #     Add to chunk file only those tilesets that will be used.
    #     Iterate over elevantions in chunk_data and find tilesets paths by GIDs
    #     """
    #     required_tilesets = set()

    #     for elevation in chunk_data["elevations"]:
    #         for layer in elevation:
    #             for gid in layer:
    #                 tileset_data = self.get_tileset_from_gid(gid)
    #                 if tileset_data is not None:
    #                     tileset_data_idx = self.tilesets_data.index(tileset_data)
    #                     required_tilesets.add(tileset_data_idx)

    #     chunk_data["tilesets"] = list(required_tilesets)

    #     chunk_items = []
    #     for elevation in chunk_data["elevations"]:
    #         for layer in elevation:
    #             for item in layer:
    #                 if isinstance(item, list):
    #                     raise ValueError("is list")
    #                 chunk_items.append(item)

    #     chunk_data["tiles_count"] = len([item for item in chunk_items if item != self.void_tile_gid])

    #     return chunk_data

