import unittest
import OceanConfigParser
import OceanModel
import io
RAND_CONFIG = """
[Ocean]
size=10
start_state_mode=random
probabilities=[0.1, 0.2, 0, 0.7]
[Predators]
feeding_time=10
reproduction_time=2
[Preys]
reproduction_time=1
[Random]
seed=123
"""
CONFIG = """
[Ocean]
size=10
start_state_mode=input
barriers= [(0,2), (0,3), (0,4), (0,5), (0,6), (2,0), (3,0), (4,0), (5,0), (6,0)]
[Predators]
feeding_time=10
reproduction_time=2
predators=[(0,0), (0,1), (2,2), (3,3)]
[Preys]
reproduction_time=1
preys=[(1,1), (2,3), (5,6), (4,4), (6,6)]
[Random]
seed=123
"""


class TestOceanConfigParser(unittest.TestCase):

    def test_random_start_state(self):
        parser = OceanConfigParser.OceanConfigParser(io.BytesIO(RAND_CONFIG))
        self.assertEqual(parser.size, 10)
        self.assertEqual(parser.state_mode, 'random')
        self.assertEqual(parser.probabilities, [0.1, 0.2, 0, 0.7])
        self.assertEqual(parser.predators_feeding_time, 10)
        self.assertEqual(parser.predators_reproducing_time, 2)
        self.assertEqual(parser.preys_reproducing_time, 1)
        self.assertEqual(parser.seed, 123)

    def test_load_start_state(self):
        parser = OceanConfigParser.OceanConfigParser(io.BytesIO(CONFIG))
        self.assertEqual(parser.size, 10)
        self.assertEqual(parser.state_mode, 'input')
        self.assertEqual(parser.predators, [(0, 0), (0, 1), (2, 2), (3, 3)])
        self.assertEqual(parser.preys, [(1, 1), (2, 3), (5, 6), (4, 4), (6, 6)])
        self.assertEqual(parser.barriers, [(0, 2), (0, 3), (0, 4), (0, 5), (0, 6),
                                           (2, 0), (3, 0), (4, 0), (5, 0), (6, 0)])
        self.assertEqual(parser.predators_feeding_time, 10)
        self.assertEqual(parser.predators_reproducing_time, 2)
        self.assertEqual(parser.preys_reproducing_time, 1)
        self.assertEqual(parser.seed, 123)

    def test_create_ocean_model(self):
        iterations = 10
        parser = OceanConfigParser.OceanConfigParser(io.BytesIO(RAND_CONFIG))
        ocean = parser.create_ocean_model(iterations)
        self.assertEqual(ocean.predators_feeding_time, 10)
        self.assertEqual(ocean.predators_reproducing_time, 2)
        self.assertEqual(ocean.preys_reproducing_time, 1)
        self.assertEqual(ocean.iterations, iterations)

    def test_random_cell_type(self):
        parser = OceanConfigParser.OceanConfigParser(io.BytesIO(RAND_CONFIG))
        cell_type = parser.get_random_cell_type()
        self.assertTrue(cell_type == OceanModel.Predator or
                        cell_type == OceanModel.Prey or
                        cell_type == OceanModel.Barrier or
                        cell_type == OceanModel.Empty)

if __name__ == '__main__':
    unittest.main()
