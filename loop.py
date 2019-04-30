import matplotlib.pyplot as plt
from edge import Edge, EdgeList
from point import Point, PointList

class Loop:
    def __init__(self, points):
        l = len(points)
        self.center = sum(points, Point(0, 0)) / l

        points.sort(key = lambda p: (p - self.center).angle)
        self.points = PointList(*points) 
        self.edges = EdgeList()
        for i in range(len(self.points)):
            self.edges.append(Edge(self.points[i - 1], self.points[i]))

    def _plot(self, style='k'):
        for edge in self.edges:
            edge._plot(style)

    def plot(self, style='k'):
        self._plot(style)
        plt.axis("square")
        plt.show()

    def intersection(self, loop):
        intersections = []
        for edge_a in self.edges:
            for edge_b in loop.edges:
                ixn = edge_a.intersection(edge_b)
                if ixn:
                    intersections.append((ixn, edge_a, edge_b))
        return intersections

    def union(self, loop):
        ixns = self.intersection(loop)
        points = PointList()

        broken_self_points = PointList(*self.points, *[i[0] for i in ixns])
        broken_loop_points = PointList(*loop.points, *[i[0] for i in ixns])

        new_self = Loop(broken_self_points)
        new_loop = Loop(broken_loop_points)

        new_edges = EdgeList(*new_self.edges)

        new_ixns = new_self.intersection(new_loop)
        replacements = []
        while len(new_ixns) > 1:
            p_ixn, p_edge_a, p_edge_b = new_ixns[0]
            n_ixn, n_edge_a, n_edge_b = new_ixns[1]
            new_ixns.pop(0)

            replacements.append([(p_edge_a, n_edge_a), (p_edge_b, n_edge_b)])

            p_j = new_loop.edges.index(p_edge_b) 
            n_j = new_loop.edges.index(n_edge_b)

            replacement = new_loop.edges[p_j:n_j]

            try:
                p_i = new_edges.index(p_edge_a) 
                n_i = new_edges.index(n_edge_a)
                new_edges[p_i+1:n_i] = replacement
            except Exception as e:
                print(p_edge_a, len(new_ixns))
                raise e

        points = PointList()
        for edge in new_edges:
            for point in edge.points:
                points.append(point)

        return Loop(points)

import math

square = Loop([Point(0.1,0.1), Point(0.9,0.1), Point(0.9,0.9), Point(0.1,0.9)])

dt = 8
circle = Loop([Point(
    0.5 * (math.cos(2 * math.pi * t/dt) + 1), 
    0.5 * (math.sin(2 * math.pi * t/dt) + 1)
) for t in range(dt)])

square._plot('r--')
circle._plot('g--')
square.union(circle).plot()

square._plot('r--')
circle._plot('g--')
circle.union(square).plot()
