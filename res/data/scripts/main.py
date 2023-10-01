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

    """ Checksum chunks and world data """
    print("-------------------------------------------------------------")
    print("Checksum:")

    min_elevation_idx = -8
    max_elevation_idx = 8
    min_layer_idx = 0
    max_layer_idx = 6

    map_data_gids_counts = {}
    chunks_gids_counts = {}
    for elevation_idx in range(min_elevation_idx, max_elevation_idx + 1):
        map_data_gids_counts[elevation_idx] = {}
        chunks_gids_counts[elevation_idx] = {}
        for layer_idx in range(min_layer_idx, max_layer_idx + 1):

            """ count gids not void and not default """
            map_data_level_gids_counts = {}
            chunks_level_gids_counts = {}

            """ count gids of maps datas """
            for map_data in maps_data:
                map_data_elevation = map_data.get_elevation_at(elevation_idx)
                if map_data_elevation is None:
                    continue
                map_data_layer = map_data_elevation.get_layer_at(layer_idx)
                if map_data_layer is None:
                    continue
                m_gids_counts = map_data_layer.get_gids_counts([config.default_tile_gid, config.void_tile_gid])
                # remove void and default

                for gid, count in m_gids_counts.items():
                    if gid not in map_data_level_gids_counts.keys():
                        map_data_level_gids_counts[gid] = 0
                    map_data_level_gids_counts[gid] += count
            map_data_gids_counts[elevation_idx][layer_idx] = map_data_level_gids_counts

            """ count gids of chunks """
            for chunk in chunks:
                chunk_elevation = chunk.get_elevation_at(elevation_idx)
                if chunk_elevation is None:
                    continue
                chunk_layer = chunk_elevation.get_layer_at(layer_idx)
                if chunk_layer is None:
                    continue
                ch_gids_counts = chunk_layer.get_gids_counts([config.default_tile_gid, config.void_tile_gid])
                # remove void and default

                for gid, count in ch_gids_counts.items():
                    if gid not in chunks_level_gids_counts.keys():
                        chunks_level_gids_counts[gid] = 0
                    chunks_level_gids_counts[gid] += count
            chunks_gids_counts[elevation_idx][layer_idx] = chunks_level_gids_counts

            if elevation_idx == 3:
                pass

    """ compare """
    test_gids_sets_passed = True
    test_gids_counts_passed = True
    for elevation_idx in range(min_elevation_idx, max_elevation_idx + 1):
        map_data_elevation_gids_counts = map_data_gids_counts[elevation_idx]
        chunks_elevation_gids_counts = chunks_gids_counts[elevation_idx]

        for layer_idx in range(min_layer_idx, max_layer_idx + 1):
            map_data_level_gids_counts = map_data_elevation_gids_counts[layer_idx]
            chunks_level_gids_counts = chunks_elevation_gids_counts[layer_idx]

            map_data_gids = set(map_data_level_gids_counts.keys())
            chunks_gids = set(chunks_level_gids_counts.keys())
            if map_data_gids != chunks_gids:
                # diff_gids = map_data_gids.symmetric_difference(chunks_gids)
                test_gids_sets_passed = False
                print(f"Error @ {elevation_idx},{layer_idx}: map data gids != chunks gids, diff: {map_data_gids} vs. {chunks_gids} - FAILED!")
            
            gids_keys = list(map_data_gids.intersection(chunks_gids))
            for gid in gids_keys:
                if map_data_level_gids_counts[gid] != chunks_level_gids_counts[gid]:
                    print(f"Error @ {elevation_idx},{layer_idx}: count of {gid}: map data gids [{map_data_level_gids_counts[gid]}] != chunks gids [{chunks_level_gids_counts[gid]}]")
                    test_gids_counts_passed = False

    if not test_gids_sets_passed:
        print("GIDs sets equal - FAIELD!")
        abort_and_exit()
    print("GIDs sets equal - PASSED!")

    if not test_gids_counts_passed:
        print("GIDs counts equal - FAIELD!")
        abort_and_exit()
    print("GIDs counts equal - PASSED!")

    """ Save chunks and world data """
