import ConfigParser
import OceanModel
import random


class OceanConfigParser:
    def __init__(self, config):
        self.parser = ConfigParser.RawConfigParser()
        self.parser.readfp(config)
        self.size = 0
        self.predators = []
        self.preys = []
        self.barriers = []

        self.size = self.parser.getint("Ocean", "size")
        self.predators_feeding_time = self.parser.getint("Predators", "feeding_time")
        self.predators_reproducing_time = self.parser.getint("Predators", "reproduction_time")
        self.preys_reproducing_time = self.parser.getint("Preys", "reproduction_time")
        self.seed = self.parser.getint("Random", "seed")
        self.state_mode = self.parser.get("Ocean", "start_state_mode")
        if self.state_mode == "input":
            self.load_start_state()
        else:
            probabilities_string = self.parser.get("Ocean", "probabilities")
            self.probabilities = eval(probabilities_string)
            self.generate_random_start_state()

    def validate(self):
        assert self.size > 0
        assert self.predators_feeding_time > 0
        assert self.predators_reproducing_time > 0
        assert self.preys_reproducing_time > 0
        assert self.seed > 0
        self.validate_cells(self.predators)
        self.validate_cells(self.preys)
        self.validate_cells(self.barriers)
        if self.state_mode == "random":
            assert sum(self.probabilities) == 1

    def validate_cells(self, cells):
        for cell in cells:
            assert cell[0] < self.size
            assert cell[1] < self.size
            assert cell[0] >= 0
            assert cell[1] >= 0

    def create_ocean_model(self, iterations):
        self.validate()
        random.seed(self.seed)
        return OceanModel.Ocean(self.size,
                                iterations,
                                self.predators,
                                self.preys,
                                self.barriers,
                                self.predators_reproducing_time,
                                self.predators_feeding_time,
                                self.preys_reproducing_time)

    def load_start_state(self):
        self.predators = eval(self.parser.get("Predators", "predators"))
        self.preys = eval(self.parser.get("Preys", "preys"))
        self.barriers = eval(self.parser.get("Ocean", "barriers"))

    def generate_random_start_state(self):
        for x in xrange(self.size):
            self.barriers.append((0, x))
            self.barriers.append((x, 0))
            self.barriers.append((self.size - 1, x))
            self.barriers.append((x, self.size - 1))

        for x in xrange(1, self.size - 1):
            for y in xrange(1, self.size - 1):
                cell = (x, y)
                cell_type = self.get_random_cell_type()
                if cell_type == OceanModel.Predator:
                    self.predators.append(cell)
                elif cell_type == OceanModel.Prey:
                    self.preys.append(cell)
                elif cell_type == OceanModel.Barrier:
                    self.barriers.append(cell)

    def get_random_cell_type(self):
        cell_types = {0: OceanModel.Predator, 1: OceanModel.Prey,
                      2: OceanModel.Barrier, 3: OceanModel.Empty}
        point = random.random()
        left_bound = 0.0
        for i, probability in enumerate(self.probabilities):
            if left_bound <= point < left_bound + probability:
                    return cell_types[i]
            left_bound += probability
