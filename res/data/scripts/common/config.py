import json
import os

class Config:
    def __init__(self):
        self.maps_dir = None
        self.tilesets_dir = None
        self.chunk_height = None
        self.chunk_width = None
        self.default_tile_gid = None
        self.void_tile_gid = None
        self.output_root_dir = None
        self.forward_slash_path = None
        self.output_data_dir = None
        self.output_world_file = None
        self.json_indent = None
        self.json_format = None
        self.gid_infinity = None

    @staticmethod
    def load(filepath) -> 'Config':
        config_dict = None

        with open(filepath, 'r') as config_file:
            config_dict = json.load(config_file)

        if config_dict is None:
            return None
        
        config = Config()

        config.maps_dir = config_dict["maps_dir"]
        config.tilesets_dir = None
        config.chunk_height = config_dict["chunk_height"]
        config.chunk_width = config_dict["chunk_width"]
        config.default_tile_gid = config_dict["default_tile_gid"]
        config.void_tile_gid = config_dict["void_tile_gid"]
        config.output_root_dir = config_dict["output_root_dir"]
        config.forward_slash_path = config_dict['forward_slash_path']
        config.output_data_dir = os.path.join(config_dict["output_root_dir"], 
                                              config_dict["output_data_dir_name"])
        config.output_world_file = config_dict["output_world_file"]
        config.json_indent = 4 if config_dict["fancy_json"] is True else None
        config.json_format = (", ", ": ") if config_dict["fancy_json"] is True else (",", ":")
        config.gid_infinity = config_dict["gid_infinity"]

        return config
    
    def __str__(self):
        return f"""Config(
    maps_dir={self.maps_dir},
    tilesets_dir={self.tilesets_dir},
    chunk_height={self.chunk_height},
    chunk_width={self.chunk_width},
    default_tile_gid={self.default_tile_gid},
    void_tile_gid={self.void_tile_gid},
    output_root_dir={self.output_root_dir},
    forward_slash_path={self.forward_slash_path},
    output_data_dir={self.output_data_dir},
    output_world_file={self.output_world_file},
    json_indent={self.json_indent},
    json_format={self.json_format},
    gid_infinity={self.gid_infinity}
)"""