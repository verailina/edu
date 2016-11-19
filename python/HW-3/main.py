import OceanConfigParser
import argparse

def main():

    parser = argparse.ArgumentParser(description='Modeling the ocean with predators and preys.')
    parser.add_argument('--config', required=True)
    parser.add_argument('--iterations', type=int, required=True)
    parser.add_argument('--printing', action='store_true')
    args = parser.parse_args()

    with open(args.config) as config_file:
        config_parser = OceanConfigParser.OceanConfigParser(config_file)
        ocean = config_parser.create_ocean_model(args.iterations)
        ocean.modeling(args.printing)

if __name__ == '__main__':
    main()
