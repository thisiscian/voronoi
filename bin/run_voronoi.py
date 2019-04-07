#!/usr/bin/python

import sys
import math
import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt
import numpy as np
import random
import pyvoronoi as v

def parameterised_heart(t):
	T = t * 2 * math.pi;
	x = 16 * pow(math.sin(T), 3)
	y = 13 * math.cos(T) - 5 * math.cos(2 * T) - 2 * math.cos(3 * T) - math.cos(4 * T)
	return [x / 16.0, y / 16.0]

data = [parameterised_heart(t/100.0) for t in range(0, 100)]
xdata = [d[0] for d in data]
ydata = [d[1] for d in data]

def point_dist(a, b):
    x, y = a
    X, Y = b
    return math.sqrt(pow(x - X, 2) + pow(y - Y, 2))

random_points = []
for x in range(1000):
    x, y = parameterised_heart(random.uniform(0, 1))
    r = random.uniform(0, 1)
    p = [r * x, r * y]
    does_pass = True
    for point in random_points:
        dist = point_dist(p, point)
        if dist < 0.1:
            does_pass = False
            continue
    if does_pass:
        random_points.append(p)
    
points = [v.Point(*d) for d in random_points]
solved = v.solve(points)

pxdata = [d[0] for d in random_points]
pydata = [d[1] for d in random_points]
plt.plot(pxdata, pydata, 'rh', xdata, ydata, 'k', markersize=2)

directrix = 2 * solved.unsolved[-1].y - solved.unsolved[-2].y
for focus in solved.unsolved:
    parabola = v.Parabola(focus)
    quadratic = parabola.to_quadratic(directrix)

    pxdata = [x / 100.0 for x in range(-100, 100)]
    pydata = [quadratic.get_y(x) for x in pxdata]

    for i in reversed(range(len(pxdata))):
        if pydata[i] > 1.0 or pydata[i] < min(-1, directrix):
            pydata.pop(i)
            pxdata.pop(i)

    plt.plot(pxdata, pydata, 'g--')

plt.plot([-1, 1], [directrix, directrix])

plt.axis('off')
plt.show()

