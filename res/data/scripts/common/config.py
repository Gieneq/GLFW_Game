import json
import os

class Config:
    def __init__(self):
        self.workspace_dirpath = None

        self.maps_dirpath = None
        self.output_root_dirpath = None
        self.output_data_dirpath = None
        self.output_world_filepath = None
        self.output_visualization_filepath = None

        self.chunk_height = None
        self.chunk_width = None
        
        self.default_tile_gid = None
        self.void_tile_gid = None
        self.gid_infinity = None

        self.use_forward_slash = True

        self.json_indent = None
        self.json_format = None

        self.verbose = False

    @staticmethod
    def load(workspace_dirpath) -> 'Config':
        config_dict = None

        config_filepath = os.path.join(workspace_dirpath, 'config.json')

        with open(config_filepath, 'r') as config_file:
            config_dict = json.load(config_file)

        if config_dict is None:
            return None
        
        config = Config()

        config.workspace_dirpath = workspace_dirpath
        config.maps_dirpath = os.path.join(workspace_dirpath, config_dict["maps_dir"])
        config.maps_dirpath = os.path.normpath(config.maps_dirpath)

        config.output_root_dirpath = os.path.join(workspace_dirpath, config_dict["output_root_dir"])
        config.output_root_dirpath = os.path.normpath(config.output_root_dirpath)
        config.output_data_dirpath = os.path.join(config.output_root_dirpath, config_dict["output_data_dir"])
        config.output_data_dirpath = os.path.normpath(config.output_data_dirpath)
        config.output_world_filepath = os.path.join(config.output_root_dirpath, config_dict["output_world_filename"])
        config.output_world_filepath = os.path.normpath(config.output_world_filepath)
        config.output_visualization_filepath = os.path.join(config.output_root_dirpath, config_dict["output_visualization_file"])
        config.output_visualization_filepath = os.path.normpath(config.output_visualization_filepath)

        config.chunk_height = config_dict["chunk_height"]
        config.chunk_width = config_dict["chunk_width"]

        config.default_tile_gid = config_dict["default_tile_gid"]
        config.void_tile_gid = config_dict["void_tile_gid"]
        config.gid_infinity = config_dict["gid_infinity"]

        config.use_forward_slash = config_dict['use_forward_slash']

        config.json_indent = 4 if config_dict["fancy_json"] is True else None
        config.json_format = (", ", ": ") if config_dict["fancy_json"] is True else (",", ":")

        return config
    
    def __str__(self):
        return f"""Config(
    workspace_dirpath={self.workspace_dirpath},
    maps_dirpath={self.maps_dirpath},
    output_root_dirpath={self.output_root_dirpath},
    output_data_dirpath={self.output_data_dirpath},
    output_world_filepath={self.output_world_filepath},
    output_visualization_filepath={self.output_visualization_filepath},
    chunk_height={self.chunk_height},
    chunk_width={self.chunk_width},
    default_tile_gid={self.default_tile_gid},
    void_tile_gid={self.void_tile_gid},
    forward_slash_path={self.use_forward_slash},
    json_indent={self.json_indent},
    json_format={self.json_format},
    gid_infinity={self.gid_infinity}
)"""