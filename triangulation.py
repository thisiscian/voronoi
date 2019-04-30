import math
import matplotlib.pyplot as plt

from edge import Edge, EdgeList
from point import Point, PointList
from triangle import Triangle, TriangleList

class Triangulation(object):
    def __init__(self, points):
        self.points = PointList(*points)
        self._update_convex_hull()
        self._update_triangles()

    def _update_convex_hull(self):
        S = sorted(self.points)
        self.convex_hull = PointList()

        pointOnHull = S[0]
        prev = Point(-1, 0).angle
        math.atan2(0, -1)
        while True:
            self.convex_hull.append(pointOnHull)
            if(len(self.convex_hull) > 1):
                rel = self.convex_hull[-2] - pointOnHull
                prev = rel.angle

            endpoint = max([p for p in S if p != pointOnHull], key = lambda p: (((p - pointOnHull).angle - prev) % ( 2 * math.pi), -pointOnHull.distance(p)))
            prev = (endpoint - pointOnHull).angle
            pointOnHull = endpoint

            if endpoint == self.convex_hull[0]:
                break

        self.convex_hull_edges = EdgeList(*[Edge(self.convex_hull[i - 1], self.convex_hull[i]) for i in range(len(self.convex_hull))])

    def _update_triangles(self):
        self.triangles = TriangleList()
        unused_points = PointList(*[p for p in self.points if p not in self.convex_hull])
        unused_points.sort(key = lambda p: (p - self.convex_hull[0]).angle)

        for i in range(1, len(self.convex_hull) - 1):
            self.triangles.append(Triangle(self.convex_hull[0], self.convex_hull[i], self.convex_hull[i + 1]))

        for point in unused_points:
            for triangle in self.triangles:
                if triangle.contains(point):
                    self.triangles.remove(triangle)
                    for edge in triangle.edges:
                        edge.triangles.remove(triangle)
                        self.triangles.append(Triangle(point, *edge))
                    break

        self.edges = EdgeList(*self.triangles)

    def plot(self):
        for edge in self.edges:
            plt.plot([edge.start.x, edge.stop.x], [edge.start.y, edge.stop.y], 'k--')

        plt.scatter([p[0] for p in self.points], [p[1] for p in self.points], c='k', marker='.')

        plt.axis("square")
        plt.axis([0,1,0,1])
        plt.show()


class DelaunayTriangulation(Triangulation):
    def _update_triangles(self):
        super()._update_triangles()

        stack = []
        marked = []

        internal_edges = EdgeList(*[ e for e in self.edges if e not in self.convex_hull_edges])

        for edge in internal_edges: 
            stack.append(edge)
            marked.append(edge)

        i = 0
        while stack:
            i+=1
            edge = stack.pop(0)
            marked.remove(edge)

            if edge.has_local_delaunayhood:
                continue

            others = []
            while edge.triangles:
                tri = edge.triangles[0]
                self.triangles.remove(tri)

                for e in tri.edges:
                    e.triangles.remove(tri)

                others.append([p for p in tri.points if p not in edge][0])

            internal_edges.remove(edge)
            new_edge = Edge(*others)
            internal_edges.append(new_edge)

            for p in edge.points:
                new_triangle = Triangle(p, *others)
                self.triangles.append(new_triangle)

            for triangle in new_edge.triangles:
                for e in triangle.edges:
                    if e not in self.convex_hull_edges and e not in marked and e != new_edge:
                        stack.append(e)
                        marked.append(e)

        self.edges = EdgeList(*self.triangles)

class Voronoi:
    def __init__(self, points, boundary):
        self.points = points
        self.delaunay = DelaunayTriangulation(self.points)
        self.boundary = boundary

        extra_boundary_points = self.extend_to_boundary()
        self.expanded_boundary = PointList(*boundary, *extra_boundary_points)
        self.expanded_boundary.sort(key = lambda P: (P.angle, P.sq_length if P.angle < math.pi else 1.0/P.sq_length))
        print(self.expanded_boundary)
        print([(P.angle, P.sq_length if P.angle < math.pi else 1.0/P.sq_length) for P in self.expanded_boundary])
        new_points = PointList(
            *points,
            #*boundary,
            *self.expanded_boundary,
        )

        self.expanded = DelaunayTriangulation(new_points)
        self.get_polygons()


    def extend_to_boundary(self):
        line_equations = []
        for i in range(len(self.boundary)):
            start = self.boundary[i - 1]
            stop = self.boundary[i]

            line = stop - start

            if line.x != 0:
                a = line.y / line.x
                line_eq = [a, start.y - start.x * a]
            else:
                line_eq = [start.x]

            line_equations.append(line_eq)

        extra_points = []
        for e in self.delaunay.convex_hull_edges:
            midpoint = (e.start + e.stop) / 2.0
            circumcenter = e.triangles[0].circumcenter
            ray = (midpoint, circumcenter - midpoint)

            # y = ax + b : 
            if ray[1].x != 0:
                a = ray[1].y / ray[1].x
                ray_eq = [a, midpoint.y - midpoint.x * a]
            else:
                ray_eq = [midpoint.x]

            ys = sorted([e.start.y, e.stop.y])
            xs = sorted([e.start.x, e.stop.x])
        
            point = None
            for i in range(len(line_equations)):
                line_eq = line_equations[i]

                if len(ray_eq) == 1 and len(line_eq) == 1 or line_eq[0] == ray_eq[0]:
                    continue

                elif len(ray_eq) == 1:
                    x = ray_eq[0]
                    y = line_eq[0] * x + line_eq[1]
                elif len(line_eq) == 1:
                    x = line_eq[0]
                    y = ray_eq[0] * x + ray_eq[1]
                else:
                    x = (ray_eq[1] - line_eq[1]) / (line_eq[0] - ray_eq[0])
                    y = line_eq[0] * x + line_eq[1]

                P = Point(x,y)

                if len(line_eq) == 1:
                    if ys[0] <= y and y <= ys[1]:
                        if point is None or P.distance(midpoint) < point.distance(midpoint):
                            point = P
                else:
                    if xs[0] <= x and x <= xs[1]:
                        if point is None or P.distance(midpoint) < point.distance(midpoint):
                            point = P

            if point:
                extra_points.append(point)

        return extra_points

    def get_polygons(self):
        groups = {p: PointList() for p in self.points}
        for edge in self.delaunay.edges:
            points = [t.circumcenter for t in edge.triangles]
            for p in edge.points:
                for P in points:
                    groups[p].append(P)

        for p, group in groups.items():
            group.sort(key = lambda P: (p - P).angle)

        self.polygons = groups
        return

    def plot(self):
        plt.scatter([p[0] for p in self.points], [p[1] for p in self.points], c='k', marker='.')

        for point, polygon in self.polygons.items():
            if polygon:
                plt.plot([p[0] for p in polygon + [polygon[0]]], [p[1] for p in polygon + [polygon[0]]], 'k')
                plt.fill([p[0] for p in polygon], [p[1] for p in polygon], zorder=-10)

        #for triangle in self.delaunay.triangles:
        #    c = triangle.circumcenter
        #    r = c.distance(triangle.points[0])
        #    t = [ i * 2 * math.pi / 180 for i in range(180)]
        #    plt.plot([r * math.cos(T) + c.x for T in t], [r * math.sin(T) + c.y for T in t], '-.' )

        #for edge in self.dual.edges:
        #    plt.plot([edge.start.x, edge.stop.x], [edge.start.y, edge.stop.y], 'b--')

        for edge in self.delaunay.edges:
            plt.plot([edge.start.x, edge.stop.x], [edge.start.y, edge.stop.y], 'k-.')
            
        plt.axis("square")
        plt.axis([0,1,0,1])
        plt.show()
