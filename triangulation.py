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
        self.get_polygons()

    def get_intersection(self, ray_start, ray_direction):
        for data in self.get_intersection_and_boundary_index(ray_start, ray_direction):
            if data:
                return data[0]
            else:
                return data

    def get_intersection_and_boundary_index(self, ray_start, ray_direction):
        # y = m * x + c
        for i in range(len(self.boundary)):
            start = self.boundary[i - 1]
            stop = self.boundary[i]

            v1 = ray_start - start
            v2 = stop - start
            v3 = Point(-ray_direction.y, ray_direction.x)

            dot = v2 * v3
            if abs(dot) == 0:
                continue

            t1 = v2.cross(v1) / dot
            t2 = (v1 * v3) / dot
            
            if t1 >= 0 and (t2 >= 0 and t2 <= 1.0):
                ixn = t1 * ray_direction + ray_start;
                yield ixn, i - 1


    def get_polygons(self):
        groups = {p: PointList() for p in self.points}
        edge_points = {}
        l = len(self.delaunay.convex_hull)
        s = sum(self.delaunay.convex_hull, Point(0, 0)) / l
        for edge in self.delaunay.edges:
            points = [t.circumcenter for t in edge.triangles]
            midpoint = (edge.start + edge.stop) / 2.0

            for p in edge.points:
                for P in points:
                    groups[p].append(P)
                    
                    if len(edge.triangles) == 1:
                        dxn = midpoint - P
                        if s.distance(midpoint) < s.distance(P):
                            dxn = -1 * dxn
                        ixn = self.get_intersection(P, dxn)
                        groups[p].append(ixn)


        for p, group in groups.items():
            group.sort(key = lambda P: (p - P).angle)

        self.polygons = groups
        return

    def plot(self):
        plt.scatter([p[0] for p in self.points], [p[1] for p in self.points], c='k', marker='.')
        plt.plot([p.x for p in self.boundary], [p.y for p in self.boundary], 'k')


        for point, polygon in self.polygons.items():
            if polygon:
                plt.plot([p[0] for p in polygon + [polygon[0]]], [p[1] for p in polygon + [polygon[0]]], 'k--')
                plt.fill([p[0] for p in polygon], [p[1] for p in polygon], zorder=-10)

        #plt.plot([p[0] for p in self.boundary], [p[1] for p in self.boundary], 'k')


            
        plt.axis("square")
        plt.axis([0,1,0,1])
        plt.show()
