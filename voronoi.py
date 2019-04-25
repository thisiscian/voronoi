#!/usr/bin/python

import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt

import pyvoronoi as voronoi

solution = voronoi.solve(4)

for x_data, y_data in solution['edges']:
    plt.plot(x_data, y_data, '-')

for x_data, y_data in solution['unsolved']:
    plt.plot(x_data, y_data, '--')

plt.scatter(*solution['points'])

#plt.axis('off')
plt.axis([0,1,0,1])
plt.show()
