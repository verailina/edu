import sys
import functools

class TakesDecorator(object):
    def __init__(self, *args):
        self.argument_types = args

    def __call__(self, function):
        @functools.wraps(function)
        def decorator(*args, **kwargs):
            for type, value in zip(self.argument_types, args):
                if not isinstance(value, type):
                    raise TypeError(value + "is not type of " + str(type))
            return function(*args, **kwargs)
        return decorator

takes = TakesDecorator
exec(sys.stdin.read())