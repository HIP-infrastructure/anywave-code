class Marker:
    """description of class"""
    def __init__(self):
        label = ''
        position = 0.
        duration = 0.
        value = 0.
        targets = []

    def print(self):
        print("label=", self.label)
        print("position=", self.position)
        print("duration=", self.duration)
        print("value=", self.value)
        print("targets=", self.targets)