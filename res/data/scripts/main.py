import argparse
import shutil
import os
from common.config import Config
from map.loader import MapsLoader
from chunking.processor import ChunkingProcessor

def abort_and_exit():
    print('Aborting with error...')
    exit(1)


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
    

if __name__ == '__main__':
    print('Map converter for Tiled Map editor. As a result world streaming capable format.')

    workspace_dir = 'res/data/scripts/'
    verbosity_level = 0
    parser = argparse.ArgumentParser(description='Description of your script.')
    parser.add_argument('-p', '--path', type=str, help='Path to workspace dir')
    parser.add_argument('-v', '--verbose', action='count', default=0, help='Verbosity level')
    args = parser.parse_args()
    if args.path:
        workspace_dir = args.path
    if args.verbose:
        verbosity_level = args.verbose

    print(f'Starting from workspace at: {workspace_dir}')

    """ Load config file """
    config = Config.load(workspace_dir)
    if config is None:
        abort_and_exit()
        
    if verbosity_level > 0:
        config.verbose = True

    if config.verbose:
        print(config, end="\n\n")

    """ Clear output dirs """
    # if not remove_dir_if_possible(config.output_root_dir):
    #     print("Removing output directory failed")
    #     return False

    # if not create_dir_if_possible(config.output_root_dir):
    #     print("Creating output directory failed")
    #     return False

    # if not create_dir_if_possible(config.output_data_dir):
    #     print("Creating output data directory failed")
    #     return False

    """ Load all maps data """
    maps_loader = MapsLoader(config)
    maps_data = maps_loader.validateAndloadAllMapsData()
    if maps_data is None or len(maps_data) == 0:
        abort_and_exit()
        
    if config.verbose:
        print("-------------------------------------------------------------")
        print("Loaded maps:")
        for map_data in maps_data:
            print(map_data, end="\n\n")
    
    """ Process all maps data into chunks """
    chunker = ChunkingProcessor(config, maps_loader.tilesetsDatabase)
    chunks = chunker.splitMapsIntoChunks(maps_data)
    if chunks is None or len(chunks) == 0:
        abort_and_exit()

    if config.verbose:
        print(f"Got [{len(chunks)}] chunks, expected [{chunker.chunks_x * chunker.chunks_y}]")

    if len(chunks) != chunker.chunks_x * chunker.chunks_y:
        abort_and_exit()


    """ Visualize chunks and world data """
    print("-------------------------------------------------------------")
    print("Created chunks:")
    for chunk in chunks:
        print(chunk)
    print("")

    """ Statistics """
    print("-------------------------------------------------------------")
    print("Statistics:")
    
    # gids_counts = {}
    # for chunk in chunks:
    #     for _, elevation in chunk.elevations_map.items():
    #         elevations_layes = elevation.get_all_layers()
    ### warning - laye cna be None
    #         for layer in elevations_layes:
    #             layer_gids_counts = layer.get_gids_c


    """ Save chunks and world data """
