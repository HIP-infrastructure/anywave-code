
import numpy

class Channel:
    def __init__(self):
        name = ''
        sr = 0.
        lp = 0.
        hp = 0.
        notch = 0.
        data = numpy.empty(1, numpy.float32)
