import matplotlib.pyplot as plt
from point import Point, PointList

class Edge(object):
    __all_edges__ = []

    def __new__(cls, a, b):
        s, S = sorted([a, b])
        for edge in cls.__all_edges__:
            if edge and edge.start == s and edge.stop == S:
                return edge

        instance = super(Edge, cls).__new__(cls)
        instance.start = s
        instance.stop = S
        instance.triangles = []
        instance.points = PointList(s, S)
        cls.__all_edges__.append(instance)
        return instance

    def __eq__(self, edge):
        if isinstance(edge, Edge):
            return self.start == edge.start and self.stop == edge.stop
        return super().__eq__(edge)

    def __iter__(self):
        yield self.start
        yield self.stop
    
    def __str__(self):
        return '({:.2f}, {:.2f} <-> {:.2f}, {:.2f})'.format(*self.start, *self.stop)

    def __hash__(self):
        return hash(str(self))

    @property
    def has_local_delaunayhood(self):
        points = PointList(*self.triangles)
        for t in self.triangles:
            c = t.circumcenter
            r = c.distance(t.points[0])
            point = [p for p in points if p not in t.points][0]
            R = c.distance(point)
            if R < r:
                return False

        return True

    def _plot(self, style='k'):
        plt.plot([self.start.x, self.stop.x], [self.start.y, self.stop.y], style)

    def plot(self, style='k'):
        self._plot(style)
        plt.axis('square')
        plt.show()

    def intersection(self, edge):
        x1 = self.start.x
        x2 = self.stop.x
        x3 = edge.start.x
        x4 = edge.stop.x

        y1 = self.start.y
        y2 = self.stop.y
        y3 = edge.start.y
        y4 = edge.stop.y

        d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4)
        if d == 0:
            return None

        t = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)
        u = (y1 - y2) * (x1 - x3) - (x1 - x2) * (y1 - y3)
        
        if t / d < 0 or t / d > 1:
            return None

        if u / d < 0 or u / d > 1:
            return None

        x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d
        y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d

        return Point(x, y)


class EdgeList(list):
    def __init__(self, *items):
        for item in items:
            self.append(item)

    def append(self, item):
        if isinstance(item, Edge):
            super().append(item)
        elif hasattr(item, 'edges') and isinstance(item.edges, EdgeList):
            for edge in item.edges:
                self.append(edge)
