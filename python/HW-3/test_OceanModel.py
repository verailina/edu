import OceanModel
import unittest

CONFIG = """
[Ocean]
size=7
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
PREDATORS = [(0, 0), (0, 1), (2, 2), (3, 3), (5, 5), (6, 5)]
PREYS = [(1, 1), (2, 3), (5, 6), (4, 4), (6, 6)]
BARRIERS = [(0, 2), (0, 3), (0, 4), (0, 5), (0, 6),
            (2, 0), (3, 0), (4, 0), (5, 0), (6, 0)]


class TestPredator(unittest.TestCase):
    def test_process(self):
        predators = [(0, 0)]
        preys = [(2, 2)]
        barriers = [(1, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        predator = ocean.get_content((0, 0))
        predator.process()
        self.assertEqual(predator.cell, (0, 1))
        predator.hungry_time = 1
        predator.process()
        self.assertEqual(ocean.predators_number, 0)

        predators = [(0, 0)]
        preys = [(2, 2)]
        barriers = [(2, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        predator = ocean.get_content((0, 0))
        predator.reproduction_time = 2
        predator.process()
        self.assertEqual(ocean.predators_number, 2)

    def test_reproduction(self):
        predators = [(0, 0)]
        preys = [(2, 2)]
        barriers = [(1, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        predator = ocean.get_content((0, 0))
        predator.reproduction()
        self.assertTrue(isinstance(ocean.get_content((0, 1)), OceanModel.Predator))
        self.assertEqual(predator.cell, (0, 0))
        self.assertEqual(predator.reproduction_time, 0)

    def test_eat(self):
        predators = [(0, 0)]
        preys = [(1, 0)]
        barriers = [(2, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        predator = ocean.get_content((0, 0))
        predator.eat((1, 0))
        self.assertTrue(isinstance(ocean.get_content((1, 0)), OceanModel.Predator))
        self.assertTrue(isinstance(ocean.get_content((0, 0)), OceanModel.Empty))
        self.assertEqual(predator.hungry_time, 0)
        self.assertEqual(ocean.preys_number, 0)
        self.assertEqual(ocean.predators_number, 1)
        self.assertEqual(predator.cell, (1, 0))

    def test_move_to(self):
        predators = [(0, 0)]
        preys = [(2, 2)]
        barriers = [(1, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        predator = ocean.get_content((0, 0))
        predator.move_to((0, 1))
        self.assertTrue(isinstance(ocean.get_content((0, 1)), OceanModel.Predator))
        self.assertTrue(isinstance(ocean.get_content((0, 0)), OceanModel.Empty))
        self.assertEqual(ocean.preys_number, 1)
        self.assertEqual(ocean.predators_number, 1)
        self.assertEqual(predator.cell, (0, 1))


class TestPrey(unittest.TestCase):
    def test_process(self):
        predators = [(0, 0)]
        preys = [(2, 2)]
        barriers = [(2, 0), (1, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        prey = ocean.get_content((2, 2))
        prey.process()
        self.assertEqual(prey.cell, (2, 1))

        prey.reproduction_time = 2
        prey.process()
        self.assertEqual(ocean.preys_number, 2)

    def test_reproduction(self):
        predators = [(0, 0)]
        preys = [(2, 2)]
        barriers = [(1, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        prey = ocean.get_content((2, 2))
        prey.reproduction()
        self.assertTrue(isinstance(ocean.get_content((2, 1)), OceanModel.Prey))
        self.assertEqual(prey.reproduction_time, 0)

    def test_move_to(self):
        predators = [(0, 0)]
        preys = [(2, 2)]
        barriers = [(1, 0), (1, 1), (1, 2)]
        ocean = OceanModel.Ocean(3, 1, predators, preys, barriers)
        prey = ocean.get_content((2, 2))
        prey.move_to((2, 1))
        self.assertTrue(isinstance(ocean.get_content((2, 1)), OceanModel.Prey))
        self.assertTrue(isinstance(ocean.get_content((2, 2)), OceanModel.Empty))
        self.assertEqual(ocean.preys_number, 1)
        self.assertEqual(ocean.predators_number, 1)
        self.assertEqual(prey.cell, (2, 1))


class TestOceanModel(unittest.TestCase):
    def test_init(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        self.assertEqual(ocean.size, 7)
        self.assertEqual(ocean.iterations, 10)
        for cell in PREDATORS:
            content = ocean.ocean[cell[0]][cell[1]]
            self.assertTrue(isinstance(content, OceanModel.Predator))
        for cell in PREYS:
            content = ocean.ocean[cell[0]][cell[1]]
            self.assertTrue(isinstance(content, OceanModel.Prey))
        for cell in BARRIERS:
            content = ocean.ocean[cell[0]][cell[1]]
            self.assertTrue(isinstance(content, OceanModel.Barrier))
        for x in xrange(ocean.size):
            for y in xrange(ocean.size):
                cell = (x, y)
                if cell in PREDATORS or cell in PREYS or cell in BARRIERS:
                    continue
                content = ocean.ocean[cell[0]][cell[1]]
                self.assertTrue(isinstance(content, OceanModel.Empty))
        self.assertEqual(ocean.predators_number, len(PREDATORS))
        self.assertEqual(ocean.preys_number, len(PREYS))
        self.assertEqual(len(ocean.predators_statistics), 1)
        self.assertEqual(len(ocean.preys_statistics), 1)

    def test_get_content(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (0, 0)
        content = OceanModel.Predator(ocean, cell)
        ocean.ocean[cell[0]][cell[1]] = content
        self.assertEqual(content, ocean.get_content(cell))
        ocean.ocean[cell[0]][cell[1]] = OceanModel.Empty(ocean, cell)

    def test_set_content(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (0, 0)
        content = OceanModel.Predator(ocean, cell)
        ocean.set_content(cell, content)
        self.assertEqual(ocean.ocean[cell[0]][cell[1]], content)
        ocean.ocean[cell[0]][cell[1]] = OceanModel.Empty(ocean, cell)

    def test_add_predator(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (2, 4)
        predators_number = ocean.predators_number
        ocean.add_predator(cell)
        content = ocean.get_content(cell)
        self.assertTrue(isinstance(content, OceanModel.Predator))
        self.assertEqual(ocean.predators_number, predators_number + 1)

    def test_delete_predator(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (2, 2)
        predators_number = ocean.predators_number
        ocean.delete_predator(cell)
        content = ocean.get_content(cell)
        self.assertTrue(isinstance(content, OceanModel.Empty))
        self.assertEqual(ocean.predators_number, predators_number - 1)

    def test_add_prey(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (2, 4)
        preys_number = ocean.preys_number
        ocean.add_prey(cell)
        content = ocean.get_content(cell)
        self.assertTrue(isinstance(content, OceanModel.Prey))
        self.assertEqual(ocean.preys_number, preys_number + 1)

    def test_delete_prey(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (2, 2)
        preys_number = ocean.preys_number
        ocean.delete_prey(cell)
        content = ocean.get_content(cell)
        self.assertTrue(isinstance(content, OceanModel.Empty))
        self.assertEqual(ocean.preys_number, preys_number - 1)

    def compare_list_contents(self, list_1, list_2):
        if len(list_1) != len(list_2):
            return False
        for val in list_1:
            if val not in list_2:
                return False
        return True

    def test_get_neighbors(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (0, 0)
        neighbors = ocean.get_neighbors(cell)
        expected_neighbors = [(1, 0), (1, 1)]
        self.assertTrue(self.compare_list_contents(neighbors, expected_neighbors))

    def test_get_empty_neighbors(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (0, 0)
        neighbors = ocean.get_empty_neighbors(cell)
        expected_neighbors = [(1, 0)]
        self.assertTrue(self.compare_list_contents(neighbors, expected_neighbors))

    def test_get_rand_neighbor(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        cell = (0, 0)
        neighbors = ocean.get_neighbors(cell)
        neighbor = ocean.get_rand_neighbor(neighbors)
        self.assertTrue(neighbor.cell in neighbors)
        cell = (6, 6)
        neighbors = ocean.get_empty_neighbors(cell)
        neighbor = ocean.get_rand_neighbor(neighbors)
        self.assertEqual(neighbor, None)

    def test_update_time(self):
        ocean = OceanModel.Ocean(7, 10, PREDATORS, PREYS, BARRIERS)
        ocean.update_time()
        for row in ocean.ocean:
            for cell in row:
                if isinstance(cell, OceanModel.Predator):
                    self.assertEqual(cell.reproduction_time, 1)
                    self.assertEqual(cell.hungry_time, 1)
                if isinstance(cell, OceanModel.Prey):
                    self.assertEqual(cell.reproduction_time, 1)

    def get_simple_ocean(self):
        predators = [(0, 0)]
        preys = [(1, 1)]
        barriers = [(0, 1), (1, 0)]
        return OceanModel.Ocean(2, 3, predators, preys, barriers)

    def test_iteration(self):
        ocean = self.get_simple_ocean()
        ocean.iteration()
        self.assertTrue(isinstance(ocean.get_content((0, 1)), OceanModel.Barrier))
        self.assertTrue(isinstance(ocean.get_content((1, 0)), OceanModel.Barrier))
        self.assertTrue(isinstance(ocean.get_content((0, 0)), OceanModel.Predator))
        self.assertTrue(isinstance(ocean.get_content((1, 1)), OceanModel.Empty))

    def test_get_statistics(self):
        ocean = self.get_simple_ocean()
        ocean.modeling(False)
        self.assertEqual(ocean.predators_statistics, [1, 1])
        self.assertEqual(ocean.preys_statistics, [1, 0])

    def test_modeling(self):
        ocean = self.get_simple_ocean()
        ocean.modeling(False)
        self.assertTrue(isinstance(ocean.get_content((0, 1)), OceanModel.Barrier))
        self.assertTrue(isinstance(ocean.get_content((1, 0)), OceanModel.Barrier))
        self.assertTrue(isinstance(ocean.get_content((0, 0)), OceanModel.Predator))
        self.assertTrue(isinstance(ocean.get_content((1, 1)), OceanModel.Empty))


if __name__ == '__main__':
    unittest.main()
