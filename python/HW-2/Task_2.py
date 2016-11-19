import sys
import functools

class InexhaustableDecorator(object):

    def __init__(self, generator, *args):
        self.generator = generator
        self.args = args

    def __iter__(self):
        return self.generator(*self.args)

def inexhaustable(generator):
    @functools.wraps(generator)
    def decorated(*args, **kvargs):
        return InexhaustableDecorator(generator, *args)
    return decorated


exec(sys.stdin.read())
