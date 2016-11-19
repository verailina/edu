import random
import time
import sys
import os
import report


class CellContent(object):
    def __init__(self, ocean, cell):
        self.ocean = ocean
        self.cell = cell

    def process(self):
        pass

    def to_char(self):
        pass

    def increase_time(self):
        pass


class Empty(CellContent):
    def to_char(self):
        return '.'


class Barrier(CellContent):
    def to_char(self):
        return '#'


class Predator(CellContent):
    def __init__(self, ocean, cell):
        super(Predator, self).__init__(ocean, cell)
        self.hungry_time = 0
        self.reproduction_time = 0

    def increase_time(self):
        self.hungry_time += 1
        self.reproduction_time += 1

    def process(self):
        if self.hungry_time == self.ocean.predators_feeding_time:
            self.ocean.delete_predator(self.cell)
            return

        if self.reproduction_time > self.ocean.predators_reproducing_time:
            self.reproduction()

        neighbors = self.ocean.get_neighbors(self.cell)
        neighbor = self.ocean.get_rand_neighbor(neighbors)
        if not neighbor:
            return
        if isinstance(neighbor, Prey):
            self.eat(neighbor.cell)
        elif isinstance(neighbor, Empty):
            self.move_to(neighbor.cell)

    def reproduction(self):
        self.reproduction_time = 0
        neighbors = self.ocean.get_empty_neighbors(self.cell)
        neighbor = self.ocean.get_rand_neighbor(neighbors)
        if not neighbor:
            return
        self.ocean.add_predator(neighbor.cell)

    def eat(self, prey_cell):
        self.hungry_time = 0
        self.ocean.delete_prey(prey_cell)
        self.move_to(prey_cell)

    def move_to(self, neighbor_cell):
        self.ocean.set_content(neighbor_cell, self)
        self.ocean.set_content(self.cell, Empty(self.ocean, self.cell))
        self.cell = neighbor_cell

    def to_char(self):
        return 'X'


class Prey(CellContent):
    def __init__(self, ocean, cell):
        super(Prey, self).__init__(ocean, cell)
        self.reproduction_time = 0

    def increase_time(self):
        self.reproduction_time += 1

    def process(self):
        if self.reproduction_time > self.ocean.preys_reproducing_time:
            self.reproduction()

        neighbors = [x for x in self.ocean.get_neighbors(self.cell)
                     if not isinstance(self.ocean.get_content(x), Predator)]
        neighbor = self.ocean.get_rand_neighbor(neighbors)
        if not neighbor:
            return
        self.move_to(neighbor.cell)

    def reproduction(self):

        self.reproduction_time = 0
        neighbors = self.ocean.get_empty_neighbors(self.cell)
        neighbor = self.ocean.get_rand_neighbor(neighbors)
        if not neighbor:
            return
        self.ocean.add_prey(neighbor.cell)

    def move_to(self, neighbor_cell):
        self.ocean.set_content(neighbor_cell, self)
        self.ocean.set_content(self.cell, Empty(self.ocean, self.cell))
        self.cell = neighbor_cell

    def to_char(self):
        return 'o'


class Ocean:
    def __init__(self, size, iterations, predators, preys, barriers,
                 predators_reproducing_time=1, predators_feeding_time=1,
                 preys_reproducing_time=1):
        self.size = size
        self.iterations = iterations
        self.predators_number = 0
        self.preys_number = 0
        self.predators_reproducing_time = predators_reproducing_time
        self.predators_feeding_time = predators_feeding_time
        self.preys_reproducing_time = preys_reproducing_time
        self.ocean = [[Empty(self, (x, y)) for y in xrange(size)] for x in xrange(size)]
        for cell in predators:
            self.add_predator(cell)
        for cell in preys:
            self.add_prey(cell)
        for cell in barriers:
            self.set_content(cell, Barrier(self, cell))

        self.predators_statistics = [self.predators_number]
        self.preys_statistics = [self.preys_number]

    def get_content(self, cell):
        return self.ocean[cell[0]][cell[1]]

    def set_content(self, cell, content):
        self.ocean[cell[0]][cell[1]] = content

    def add_predator(self, cell):
        self.set_content(cell, Predator(self, cell))
        self.predators_number += 1

    def delete_predator(self, cell):
        self.set_content(cell, Empty(self, cell))
        self.predators_number -= 1

    def add_prey(self, cell):
        self.set_content(cell, Prey(self, cell))
        self.preys_number += 1

    def delete_prey(self, cell):
        self.set_content(cell, Empty(self, cell))
        self.preys_number -= 1

    def get_neighbors(self, cell):
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1),
                      (-1, -1), (1, 1), (-1, 1), (1, -1)]
        neighbors = []
        for direction in directions:
            neighbor = (cell[0] + direction[0], cell[1] + direction[1])
            if 0 <= neighbor[0] < self.size and \
                    0 <= neighbor[1] < self.size and \
                    not isinstance(self.get_content(neighbor), Barrier) and \
                    not isinstance(self.get_content(neighbor), type(self.get_content(cell))):
                neighbors.append(neighbor)
        return neighbors

    def get_rand_neighbor(self, neighbors):
        if len(neighbors) == 0:
            return
        if len(neighbors) == 1:
            return self.get_content(neighbors[0])
        cell_number = random.randint(0, len(neighbors) - 1)
        cell = neighbors[cell_number]
        return self.get_content(cell)

    def get_empty_neighbors(self, cell):
        return [x for x in self.get_neighbors(cell) if isinstance(self.get_content(x), Empty)]

    def iteration(self):
        for x in xrange(self.size):
            for y in xrange(self.size):
                self.get_content((x, y)).process()
        self.predators_statistics.append(self.predators_number)
        self.preys_statistics.append(self.preys_number)
        self.update_time()

    def update_time(self):
        for x in xrange(self.size):
            for y in xrange(self.size):
                content = self.get_content((x, y))
                content.increase_time()

    def print_ocean(self):
        time.sleep(0.5)
        os.system('cls')
        for row in self.ocean:
            for cell in row:
                sys.stdout.write(cell.to_char())
            sys.stdout.write('\n')

    def get_statistics(self):
        return self.predators_statistics, self.preys_statistics

    def modeling(self, is_print):
        if self.size > 50:
            print 'Printing mode is off: ocean size is too big'
            is_print = False
        if is_print:
            self.print_ocean()
        for i in xrange(self.iterations):
            if self.preys_number == 0 or self.predators_number == 0:
                break
            self.iteration()
            if is_print:
                self.print_ocean()
        report.prepare_report(self)

    def get_configuration(self):
        return self.size, \
               self.iterations, \
               self.predators_feeding_time, \
               self.predators_reproducing_time, \
               self.preys_reproducing_time