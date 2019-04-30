#!/usr/bin/python

import click
import math
import matplotlib
import random

matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt

from edge import Edge, EdgeList
from point import Point, PointList
from triangle import Triangle, TriangleList
from triangulation import Triangulation, DelaunayTriangulation, Voronoi

def parameterised_heart(t):
    T = t * 2 * math.pi;
    x = 16 * pow(math.sin(T), 3)
    y = 13 * math.cos(T) - 5 * math.cos(2 * T) - 2 * math.cos(3 * T) - math.cos(4 * T)
    return Point(x / 17.0, y / 17.0)

def generate_random_points(n_points):
    random_points = []
    fail_count = 0
    while n_points > 0 and fail_count < 10:
        p = random.random() * parameterised_heart(random.random())
        if not any([p.distance(P) < 0.08 for P in random_points]):
            random_points.append(p)
            yield p
            fail_count = 0
            n_points -= 1
        else:
            fail_count += 1

@click.command()
@click.argument("count", default=10)
@click.option("--seed", default=None)
@click.option("--heart/--no-heart", default = False)
def main(count, seed, heart):
    if seed is not None:
        random.seed(seed)

    if heart:
        rn= 50
        outer_boundary = [(0.5 * parameterised_heart(t/rn) + Point(0.5, 0.5)) for t in range(rn)]
        points = [(0.5 * p) + Point(0.5, 0.5) for p in generate_random_points(count)]
    else:
        outer_boundary = [Point(0,0), Point(1,0), Point(1,1), Point(0,1)]
        points = [Point(random.random(), random.random()) for i in range(count)]

    voronoi = Voronoi(points, outer_boundary)
    voronoi.plot()

if __name__ == '__main__':
    main()
