import os
import matplotlib.pyplot as pyplot
REPORT_DIRECTORY = "reports"


def prepare_report(ocean):
    if not os.path.exists(REPORT_DIRECTORY):
        os.makedirs(REPORT_DIRECTORY)

    pyplot.figure()
    pyplot.rcParams['legend.loc'] = 'best'

    predators_statistics, preys_statistics = ocean.get_statistics()
    size, iterations, predator_feed, predator_rep, prey_rep = ocean.get_configuration()

    passed_iterations = len(predators_statistics)
    time = xrange(0, passed_iterations)
    pyplot.plot(time, preys_statistics, label='Preys')
    pyplot.plot(time, predators_statistics, label='Predators')
    pyplot.xlabel('Iterations number')
    pyplot.ylabel('Objects number')
    plot_title = 'Iterations = {0}, Size = {1},\n Predators feed time = {2}, '\
                 'Predators reproduction time = {3},  Preys reproduction time = {4}'.\
                 format(passed_iterations - 1, size, predator_feed, predator_rep, prey_rep)
    pyplot.title(plot_title, fontsize=10)
    pyplot.legend()

    image_file_name = "Report_{0}_{1}_{2}_{3}_{4}.png".\
        format(passed_iterations - 1, size, predator_feed, predator_rep, prey_rep)
    pyplot.savefig(REPORT_DIRECTORY + '/' + image_file_name)