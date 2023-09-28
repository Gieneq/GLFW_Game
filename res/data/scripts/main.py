import argparse
import os
from common.config import Config


def abort_and_exit():
    print('Aborting...')
    exit(1)
    
if __name__ == '__main__':
    config_path = 'res/data/scripts/config.json'
    parser = argparse.ArgumentParser(description='Description of your script.')
    parser.add_argument('-c', '--config', type=str, help='Path to config file.')
    args = parser.parse_args()
    if args.config:
        config_path = os.path.normpath(args.config)
    print(f'Using config file: {config_path}')

    config = Config.load(config_path)

    if config is None:
        abort_and_exit()

    print('Hello, World!')
    print(config)
