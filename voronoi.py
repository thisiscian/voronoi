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

@click.command()
@click.argument("count", default=10)
@click.option("--seed", default=None)
def main(count, seed):
    outer_boundary = [Point(0,0), Point(1,0), Point(1,1), Point(0,1)]
    if seed is not None:
        random.seed(seed)
    points = [Point(random.random(), random.random()) for i in range(count)]
    voronoi = Voronoi(points, outer_boundary)
    voronoi.plot()

if __name__ == '__main__':
    main()
