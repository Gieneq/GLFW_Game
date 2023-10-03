from common.config import Config
from chunking.chunk import Chunk
from map.tileset import TilesetsDatabase
import shutil
import os
import json


def remove_dir_if_exists(directory_path) -> bool:
    if os.path.exists(directory_path):
        try:
            shutil.rmtree(directory_path)
            print(f"Directory '{directory_path}' and its contents have been removed.")
            return True
        except Exception as e:
            print(f"An error occurred: {e}")
            return False
    else:
        """ Not exists - no problem """
        return True
        

def create_dir_if_possible(directory_path) -> bool:
    try:
        os.makedirs(directory_path)
        print(f"Directory '{directory_path}' has been created.")
        return True
    except Exception as e:
        print(f"An error occurred: {e}")
        return False
    

class WorldSaving:
    def __init__(self, config:Config, tilesetsDatabase:TilesetsDatabase) -> None:
        self.config = config
        self.tilesetsDatabase = tilesetsDatabase
        self.chunk_briefs = {}
        self.locked = False
            
        """ Clear output dirs """
        if not remove_dir_if_exists(config.output_root_dirpath):
            print("Removing output directory failed")
        #     return False
        # if self.config.verbose:
        print(f"Removing {config.output_root_dirpath} succeeded.")

        if not create_dir_if_possible(config.output_root_dirpath):
            print("Creating output directory failed")
        #     return False
        print(f"Creating {config.output_root_dirpath} succeeded.")

        if not create_dir_if_possible(config.output_data_dirpath):
            print("Creating output data directory failed")
        #     return False
        print(f"Creating {config.output_data_dirpath} succeeded.")


    def save_world_data(self) -> bool:
        if not self.locked:
            return False
        
        """ Save world data to file """
        world_data = {
            "tilesets": self.tilesetsDatabase.to_dict(self.config.use_forward_slash),
            "chunks_data_dirpath": self.config.output_data_dirpath,
            "tilesets_dirpath": None,
            "chunks_paths": [{key: brief["path"]} for key, brief in self.chunk_briefs.items()],
        }

        if self.config.use_forward_slash:
            world_data["chunks_data_dirpath"] = world_data["chunks_data_dirpath"].replace("\\", "/")

        world_file_path = os.path.join(self.config.output_root_dirpath, "world.json")
        try:
            with open(world_file_path, "w") as world_file:
                json.dump(world_data, world_file, indent=4)
                print(f"World data saved to: {world_file_path}")
                return True
        except Exception as e:
            print(f"An error occurred: {e}")
            return False
        

    def save_chunk(self, chunk:Chunk) -> bool:
        """ Save chunk to file """
        chunk_key_name = f"{chunk.x}_{chunk.y}"
        chunk_file_path = os.path.join(self.config.output_data_dirpath, f"chunk_{chunk_key_name}.json")
        # //ad paths, reverse order of saving, lock and let save the world!
        try:
            with open(chunk_file_path, "w") as chunk_file:
                json.dump(chunk.to_dict(), chunk_file, indent=4)
                print(f"Chunk saved to: {chunk_file_path}")
                # return True
        except Exception as e:
            print(f"An error occurred: {e}")
            return False

        gids_counts_dict = {}

        for elevation in chunk.elevations_map.values():
            for layer in elevation.get_all_layers():
                if layer is None:
                    continue
                gids = layer.get_gids_counts(filtering=[self.config.default_tile_gid, self.config.void_tile_gid])
                for gid, count in gids.items():
                    if gid not in gids_counts_dict.keys():
                        gids_counts_dict[gid] = 0
                    gids_counts_dict[gid] += count

        chunk_brief = {
            "path": chunk_file_path,
            "counts": gids_counts_dict,
        }

        if self.config.use_forward_slash:
            chunk_brief["path"] = chunk_brief["path"].replace("\\", "/")

        if chunk_key_name in self.chunk_briefs.keys():
            print(f"Chunk '{chunk_key_name}' already saved")
            return False
        
        self.chunk_briefs[chunk_key_name] = chunk_brief
        return True

    def chunks_done(self):
        self.locked = True
    
