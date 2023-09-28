import xml.etree.ElementTree as ET
import struct
import shutil
import json
import os
from res.data.scripts.map.data import MapData, ElevationError, LayerError, PositionError
from gcommon import Rect, Tileset
from visualization import VisualizationData

config_path = "config.json"


class BuildingError(Exception):
    pass


class Loader:
    def __init__(self, conf_path):
        # config = self._load_config(conf_path)
        # self.maps_dir = config["maps_dir"]
        # self.tilesets_dir = None
        # self.chunk_height = config["chunk_height"]
        # self.chunk_width = config["chunk_width"]
        # self.default_tile_gid = config["default_tile_gid"]
        # self.void_tile_gid = config["void_tile_gid"]
        # self.output_root_dir = config["output_root_dir"]
        # self.forward_slash_path = config['forward_slash_path']
        # self.output_data_dir = os.path.join(self.output_root_dir, config["output_data_dir_name"])
        # self.output_world_file = config["output_world_file"]
        # self.tilesets_data = []
        # self.world_data = {}
        # self.json_indent = 4 if config["fancy_json"] is True else None
        # self.json_format = (", ", ": ") if config["fancy_json"] is True else (",", ":")
        # self.gid_infinity = config["gid_infinity"]
        # self.visualization = VisualizationData(self.chunk_width, self.chunk_height)
        # print("config:", config)

    # @staticmethod
    # def _load_config(filename):
    #     with open(filename, 'r') as config_file:
    #         config = json.load(config_file)
    #     return config

    @staticmethod
    def _remove_output_dir_if_exists(directory_path):
        if os.path.exists(directory_path):
            try:
                shutil.rmtree(directory_path)
                print(f"Directory '{directory_path}' and its contents have been removed.")
                return True
            except Exception as e:
                print(f"An error occurred: {e}")
                return False
        else:
            # print(f"Directory '{directory_path}' does not exist.")
            """ Not exists - no problem """
            return True

    @staticmethod
    def _create_output_dir_if_possible(directory_path):
        try:
            os.makedirs(directory_path)
            print(f"Directory '{directory_path}' has been created.")
            return True
        except Exception as e:
            print(f"An error occurred: {e}")
            return False

    def start(self):

        """
        Recreate output directory
        """
        if not self._remove_output_dir_if_exists(self.output_root_dir):
            print("Removing output directory failed")
            return False

        if not self._create_output_dir_if_possible(self.output_root_dir):
            print("Creating output directory failed")
            return False

        if not self._create_output_dir_if_possible(self.output_data_dir):
            print("Creating output data directory failed")
            return False

        """
        Find all maps files
        """
        maps_data = self._load_maps_data()
        if maps_data is None:
            return False

        """
        Process maps data.
        MapsData is Cuboid with elevation->layers-GIDs content.
        1. Check intersections - there shouldbe none
        2. Source maps should have equal resource paths -> GIDs the same ranges,
           it can be further improved by adding to final file only used resources.
           
        As a result there should be created chunk blocks.
        Each chunk block is predefined size and can have elevations, layers.
        It should have header with resources indication, x, y coords.
        
        There should be additional file to store information about all resources and chunks
        It should map x,y to corresponding chunk data
        """

        """
        Check overlapping of maps
        """
        maps_have_overlapping = MapData.map_datas_have_overlapping(maps_data)
        if maps_have_overlapping:
            print("Error - maps have overlapping")
            return False

        """ Add maps to visualization """
        for map_data in maps_data:
            self.visualization.append_map_data(map_data)

        """
        Create grid aligned bounding rect
        """
        bounding_rect_not_aligned = MapData.map_datas_get_bounding_rect(maps_data)
        bounding_rect_aligned = bounding_rect_not_aligned.get_align_to(self.chunk_width, self.chunk_height)
        chunks_x = int(bounding_rect_aligned.width / self.chunk_width)
        chunks_y = int(bounding_rect_aligned.height / self.chunk_height)
        print(
            f"World bounding: {bounding_rect_not_aligned} aligned to {bounding_rect_aligned}. Chunks: ({chunks_x}, {chunks_y})")

        self.visualization.bounding = bounding_rect_not_aligned
        self.visualization.aligned_bounding = bounding_rect_aligned

        self.world_data["bounding_x"] = bounding_rect_aligned.x
        self.world_data["bounding_y"] = bounding_rect_aligned.y
        self.world_data["bounding_z_min"] = float('inf')  # initial
        self.world_data["bounding_z_max"] = -float('inf')  # initial
        self.world_data["bounding_w"] = bounding_rect_aligned.width
        self.world_data["bounding_h"] = bounding_rect_aligned.height
        self.world_data["bounding_d"] = None  # initial
        self.world_data["chunks_in_x"] = chunks_x
        self.world_data["chunks_in_y"] = chunks_y
        self.world_data["chunks_data"] = []
        self.world_data["chunks_w"] = self.chunk_width
        self.world_data["chunks_h"] = self.chunk_height
        self.world_data["chunks_count"] = self.chunk_width * self.chunk_height
        self.world_data["tileset_ids_set"] = set()
        self.world_data["raw_maps_filepaths"] = list((map_data.filepath for map_data in maps_data))
        self.world_data["tiles_count"] = 0

        """
        Creation & visualization of chunks
        """
        for chunk_y in range(chunks_y):
            for chunk_x in range(chunks_x):
                window_chunk = Rect(
                    bounding_rect_aligned.x + chunk_x * self.chunk_width,
                    bounding_rect_aligned.y + chunk_y * self.chunk_height,
                    self.chunk_width,
                    self.chunk_height
                )

                # Check if window has collision with any map
                intersecting_maps = []
                for map_data in maps_data:
                    if window_chunk.intersect(map_data):
                        intersecting_maps.append(map_data)

                if len(intersecting_maps) == 0:
                    print(".", end="")
                else:
                    print(f"{len(intersecting_maps)}", end="")

                chunk_z_min = 0 if len(intersecting_maps) == 0 else min((map_data.z for map_data in intersecting_maps))
                chunk_content = self._build_chunk(window_chunk, chunk_z_min, intersecting_maps)
                if chunk_content is None:
                    return False

                self.world_data["tiles_count"] += chunk_content["tiles_count"]

                chunk_filename = f"chunk_{chunk_content['x']}_{chunk_content['y']}.json"
                chunk_filepath = os.path.join(self.output_data_dir, chunk_filename)
                chunk_filepath = self._unify_path(chunk_filepath)

                if not self._save_chunk_data(chunk_content, chunk_filepath):
                    return False

                chunk_d = len(chunk_content["elevations"])
                chunk_z_max = chunk_z_min + chunk_d

                """ Add to world data for caching """
                if chunk_z_min < self.world_data["bounding_z_min"]:
                    self.world_data["bounding_z_min"] = chunk_z_min

                if chunk_z_max > self.world_data["bounding_z_max"]:
                    self.world_data["bounding_z_max"] = chunk_z_max

                self.world_data["tileset_ids_set"] = self.world_data["tileset_ids_set"].union(chunk_content["tilesets"])
                self.world_data["chunks_data"].append({
                    "x": window_chunk.x,
                    "y": window_chunk.y,
                    "z": chunk_z_min,
                    "d": chunk_d,
                    "file": chunk_filepath,
                    "tiles_count": chunk_content["tiles_count"],
                    "tilesets": chunk_content["tilesets"],
                })

                """ Add to visualization """
                self.visualization.append_chunk(window_chunk, chunk_content["tilesets"],
                                                elevations_count=len(chunk_content["elevations"]),
                                                min_z=chunk_z_min,
                                                tiles_cout=chunk_content["tiles_count"])

            print("")

        """
        Save common world data
        """
        if not self._save_world_data():
            print("Failed to same world data")
            return False

        return True

    def _unify_path(self, filepath):
        return filepath.replace('\\', '/') if self.forward_slash_path else filepath.replace('/', '\\')

    def _build_chunk(self, window_chunk, chunk_z_min, intersecting_maps):
        chunk_data = {
            "x": window_chunk.x,
            "y": window_chunk.y,
            "z": chunk_z_min,
            "tilesets": None,
            "tiles_count": None,
            "elevations": None,
        }

        final_elevations = []
        if len(intersecting_maps) == 0:
            """
            Empty chunk - create one elevation with one layer 
            filled with default tiles global ID.
            """
            layer = [self.default_tile_gid] * self.chunk_width * self.chunk_height
            elevation = [layer]
            final_elevations.append(elevation)

        else:
            """
            Find shape of chunk - how many elevations and layers each.
            Fill them all with void values.
            """

            elevations_count = max([len(map_data.elevations) for map_data in intersecting_maps])
            for elevation_idx in range(elevations_count):
                found_elevations = []

                for map_data in intersecting_maps:
                    try:
                        elevation = map_data.elevations[elevation_idx]
                        found_elevations.append(elevation)
                    except IndexError:
                        pass

                combined_elevation = []

                layers_count = max([len(elevation) for elevation in found_elevations])
                for layer_idx in range(layers_count):
                    combined_layer = [self.void_tile_gid] * self.chunk_width * self.chunk_height
                    for iy in range(window_chunk.height):
                        for ix in range(window_chunk.width):
                            world_space_x = ix + window_chunk.x
                            world_space_y = iy + window_chunk.y

                            for map_data in intersecting_maps:
                                try:
                                    gid = map_data.get_gid_at(elevation_idx, layer_idx, world_space_x, world_space_y)
                                    if combined_layer[ix + iy * window_chunk.width] != self.void_tile_gid:
                                        raise BuildingError(f"Building chunk failed - non void tile overlapped somehow")

                                    combined_layer[ix + iy * window_chunk.width] = gid
                                except ElevationError as e_err:
                                    pass
                                except LayerError as l_err:
                                    pass
                                except PositionError as p_err:
                                    pass

                    combined_elevation.append(combined_layer)
                final_elevations.append(combined_elevation)

        chunk_data["elevations"] = final_elevations

        """
        Resources info
        Add to chunk file only those tilesets that will be used.
        Iterate over elevantions in chunk_data and find tilesets paths by GIDs
        """
        required_tilesets = set()

        for elevation in chunk_data["elevations"]:
            for layer in elevation:
                for gid in layer:
                    tileset_data = self.get_tileset_from_gid(gid)
                    if tileset_data is not None:
                        tileset_data_idx = self.tilesets_data.index(tileset_data)
                        required_tilesets.add(tileset_data_idx)

        chunk_data["tilesets"] = list(required_tilesets)

        chunk_items = []
        for elevation in chunk_data["elevations"]:
            for layer in elevation:
                for item in layer:
                    if isinstance(item, list):
                        raise ValueError("is list")
                    chunk_items.append(item)

        chunk_data["tiles_count"] = len([item for item in chunk_items if item != self.void_tile_gid])

        return chunk_data

    def _save_chunk_data(self, chunk_content, filepath):

        try:
            with open(filepath, 'w') as file:
                json.dump(chunk_content, file, indent=self.json_indent, separators=self.json_format)
                return True

        except Exception as e:
            print(f"An error occurred: {e}")
            return False

    def _save_world_data(self):
        filepath = os.path.join(self.output_root_dir, self.output_world_file)
        world_data = {
            "global_X": self.world_data["bounding_x"],
            "global_Y": self.world_data["bounding_y"],
            "global_Z": self.world_data["bounding_z_min"],
            "global_W": self.world_data["chunks_w"] * self.world_data["chunks_in_x"],
            "global_H": self.world_data["chunks_h"] * self.world_data["chunks_in_y"],
            "global_D": self.world_data["bounding_z_max"],
            "chunk_w": self.world_data["chunks_w"],
            "chunk_h": self.world_data["chunks_h"],
            "chunks_in_x": self.world_data["chunks_in_x"],
            "chunks_in_y": self.world_data["chunks_in_y"],
            "maps_dir": self.maps_dir,
            "default_gid": self.default_tile_gid,
            "void_gid": self.void_tile_gid,
            "tiles_count": self.world_data["tiles_count"],
            "tilesets_dir": self.tilesets_dir,
            "tilesets_all_ids": list(self.world_data["tileset_ids_set"]),
            "raw_maps": self.world_data["raw_maps_filepaths"],
            "tilesets": [{idx: tileset.as_dist()} for idx, tileset in enumerate(self.tilesets_data)],
            "chunks": self.world_data["chunks_data"]
        }
        try:
            with open(filepath, 'w') as file:
                json.dump(world_data, file, indent=self.json_indent, separators=self.json_format)
                return True

        except Exception as e:
            print(f"An error occurred: {e}")
            return False

    def _load_maps_data(self):
        maps_data = []

        if not (os.path.exists(self.maps_dir) and os.path.isdir(self.maps_dir)):
            return None

        files = os.listdir(self.maps_dir)
        print(f"Found {len(files)} maps in {self.maps_dir}\n")
        for file_name in files:
            file_path = os.path.join(self.maps_dir, file_name)
            if not os.path.isfile(file_path):
                return None

            file_path = self._unify_path(file_path)

            map_data = self._load_map_data(file_path)
            if map_data is None:
                print(f"Corrupted file: {file_path}")
                return None

            print(map_data)
            maps_data.append(map_data)

        return maps_data

    def _load_map_data(self, file_path):
        # Load the XML file
        tree = ET.parse(file_path)
        if tree is None:
            print(f"Corrupted file: {file_path}")
            return None

        root = tree.getroot()
        data_general = self._extract_map_data_general(file_path, root)
        data_tilesets = self._extract_map_data_tilesets(root)
        data_content = self._extract_map_data_content(data_general, root)

        if data_general is None or len(data_content) == 0 or len(data_tilesets) == 0:
            print(f"Corrupted data general, tilesets or content of: {file_path}")
            return None

        """
        Check for repeatitions
        """
        for tileset_data in data_tilesets:
            can_be_added = True
            for existing_tileset_data in self.tilesets_data:
                if existing_tileset_data == tileset_data:
                    can_be_added = False
                elif existing_tileset_data.has_in_common(tileset_data):
                    print(f"Tilesets has common gids: {existing_tileset_data} & {tileset_data}")
                    return None

            if can_be_added:
                self.tilesets_data.append(tileset_data)

        map_data = MapData(data_general)
        map_data.elevations = data_content
        map_data.tilesets = data_tilesets
        return map_data

    @staticmethod
    def _extract_map_data_general(file_path, root):
        data = {
            "filepath": file_path,
            "width": int(root.attrib['width']),
            "height": int(root.attrib['height']),
            "depth": len(root.findall(".//group"))
        }

        # Extract width and height attributes from the root element
        if data["width"] is None or data["height"] is None:
            print(f"Missing width or height: {file_path}")
            return None

        if data["depth"] is None or data["depth"] == 0:
            print(f"Corrupted depth of map: {file_path}")
            return None

        # Extract global_X and global_Y properties from the properties section
        properties = root.find(".//properties")
        data["global_x"] = int(properties.find(".//property[@name='global_X']").attrib['value'])
        data["global_y"] = int(properties.find(".//property[@name='global_Y']").attrib['value'])
        data["global_z"] = int(properties.find(".//property[@name='global_Z']").attrib['value'])

        if data["global_x"] is None or data["global_y"] is None or data["global_z"] is None:
            print(f"Missing global_x, global_y or global_z: {file_path}")
            return None

        return data


    def _extract_map_data_tilesets(self, root):
        data = []

        tilesets = root.findall(".//tileset")
        for i, tileset in enumerate(tilesets):
            first_gid = int(tileset.attrib["firstgid"])
            source = tileset.attrib["source"]
            source = os.path.join(self.maps_dir, source)
            source = os.path.normpath(source)
            source = self._unify_path(source)

            source_dir = os.path.dirname(source)
            self.tilesets_dir = source_dir
            if i < len(tilesets) - 1:
                next_tileset = tilesets[i + 1]
                last_gid = int(next_tileset.attrib["firstgid"]) - 1
            else:
                # If it's the last tileset, use a large value as the last_gid
                last_gid = self.gid_infinity

            data.append(Tileset(first_gid, last_gid, source))

        return data

    @staticmethod
    def _extract_map_data_content(data_general, root):
        elevations = []

        # Find all group elements
        group_elements = root.findall(".//group")

        # Iterate through each group element
        for group_element in group_elements:
            group_layers = []

            # Find all layer elements within the group
            layer_elements = group_element.findall(".//layer")

            # Iterate through each layer element
            for layer_element in layer_elements:
                # Find the data element within the layer
                data_element = layer_element.find(".//data")

                # Extract and parse the CSV-encoded data
                data_str = data_element.text.strip()
                layer_data = [int(value) for value in data_str.split(",")]

                group_layers.append(layer_data)

            elevations.append(group_layers)

        return elevations

    def get_tileset_from_gid(self, gid):
        for tileset_data in self.tilesets_data:
            if tileset_data.has_gid(gid):
                return tileset_data
        return None

if __name__ == '__main__':
    loader = Loader(config_path)
    loading_result = loader.start()

    if loading_result:
        print("Loading done!")
        loader.visualization.show()
    else:
        print("Loading failed!")
