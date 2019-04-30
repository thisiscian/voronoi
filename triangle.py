from edge import Edge, EdgeList
from point import Point, PointList

class Triangle(object):
    __all_triangles__ = []

    def __new__(cls, a, b, c):
        A, B, C = sorted([a, b, c])
        for triangle in cls.__all_triangles__:
            if triangle.points == PointList(A, B, C):
                return triangle

        instance = super(Triangle, cls).__new__(cls)
        instance.edges = EdgeList(Edge(A, B), Edge(B, C), Edge(C, A))
        instance.points = PointList(A, B, C)

        for edge in instance.edges:
            if instance not in edge.triangles:
                edge.triangles.append(instance)

        cls.__all_triangles__.append(instance)
        return instance

    def __eq__(self, tri):
        return sorted(tri.points) == sorted(self.points)

    @property
    def circumcenter(self):
        a, b, c = self.points
        B = b - a
        C = c - a

        D = 2 * (B.x * C.y - B.y * C.x)
        if D == 0:
            return (B + C) / 2.0

        X = (C.y * B.sq_length - B.y * C.sq_length) / D
        Y = (B.x * C.sq_length - C.x * B.sq_length) / D
        return Point(X + a.x, Y + a.y)
    
    def contains(self, point):
        def sign(a, b, c):
            ac = a - c
            bc = b - c
            return ac.cross(bc)

        a, b, c = self.points
        d1 = sign(point, a, b)
        d2 = sign(point, b, c)
        d3 = sign(point, c, a)
        has_neg = any([d < 0 for d in [d1, d2, d3]])
        has_pos = any([d > 0 for d in [d1, d2, d3]])
        return not (has_neg and has_pos)

class TriangleList(list):
    def __init__(self, *items):
        for item in items:
            self.append(item)

    def append(self, item):
        if isinstance(item, Triangle):
            super().append(item)
        elif hasattr(item, 'triangles') and isinstance(item.triangles, TriangleList):
            for triangle in item.triangles:
                self.append(triangle)
