import math

class Point(object):
    __all_points__ = []

    def __new__(cls, x, y):
        for point in cls.__all_points__:
            if point.x == x and point.y == y:
                return point

        instance = super(Point, cls).__new__(cls)
        instance.x = x
        instance.y = y
        cls.__all_points__.append(instance)
        return instance

    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __iter__(self):
        yield self.x
        yield self.y
    
    def __getitem__(self, other):
        if other == 0:
            return self.x
        elif other == 1:
            return self.y
        else:
            return super().__getitem__(other)

    def __add__(self, other):
        if isinstance(other, Point):
            return Point(self.x + other.x, self.y + other.y)
        return super().__add__(other)

    def __sub__(self, other):
        if isinstance(other, Point):
            return Point(self.x - other.x, self.y - other.y)
        return super().__sub__(other)

    def __truediv__(self, other):
        if isinstance(other, float) or isinstance(other, int): 
            return Point(self.x / other, self.y / other)
        return super().__truediv__(other)

    def __lt__(self, other):
        if isinstance(other, Point):
            return self.x < other.x or (self.x == other.x and self.y < other.y)
        return super().__lt__(other)

    def __mul__(self, other):
        if isinstance(other, float) or isinstance(other, int): 
            return Point(self.x * other, self.y * other)
        elif isinstance(other, Point):
            return self.x * other.x + self.y * other.y
        return super().__mul__(other)

    def __str__(self):
        return '({:.2f}, {:.2f})'.format(self.x, self.y)

    def __repr__(self):
        return 'Point({:.2f}, {:.2f})'.format(self.x, self.y)


    @property
    def sq_length(self):
        return self.x * self.x + self.y * self.y

    @property
    def length(self):
        return math.sqrt(self.sq_length)

    def distance(self, other):
        if isinstance(other, Point):
            return (self - other).length
            
        raise Exception('not implemented')

    def cross(self, other):
        if isinstance(other, Point):
            return self.x * other.y - self.y * other.x
        raise Exception('not implemented')

    @property
    def angle(self):
        a  = math.atan2(self.y, self.x)
        while a < 0:
            a = (a + 2 * math.pi) % ( 2 * math.pi)
        return a

class PointList(list):
    def __init__(self, *items):
        for item in items:
            self.append(item)

    def append(self, item):
        if isinstance(item, Point):
            super().append(item)
        elif hasattr(item, 'points') and isinstance(item.points, PointList):
            for point in item.points:
                self.append(point)
