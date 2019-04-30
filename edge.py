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
