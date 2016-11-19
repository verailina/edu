import sys

class AssertRaises(object):
    def __init__(self, exception_type):
        self.exception_type = exception_type

    def __enter__(self):
        return 'AssertRaises'

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type != self.exception_type:
            raise AssertionError
        return True

exec(sys.stdin.read())