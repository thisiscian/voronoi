#!/usr/bin/python

import json
import sys

import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt
import numpy

plt.axis('off')

json_string = sys.stdin.read()
json_data = json.loads(json_string)

if 'points' in json_data:
    x_data = []
    y_data = []

    for point in json_data['points']:
        x_data.append(float(point['x']))
        y_data.append(float(point['y']))

    plt.plot(x_data, y_data, 'rh', markersize=2)

if 'border' in json_data:
    x_data = []
    y_data = []

    for point in json_data['border']:
        x_data.append(float(point['x']))
        y_data.append(float(point['y']))

    plt.plot(x_data, y_data, 'k')

if 'unsolved' in json_data:
    for arc in json_data['unsolved']:
        x_data = [float(x) for x in arc['x']]
        y_data = [float(y) for y in arc['y']]

        plt.plot(x_data, y_data, c=numpy.random.rand(3,), linewidth=2.5)

if 'directrix' in json_data:
    d = json_data['directrix']
    plt.plot([-1,1], [d, d], 'g', linewidth=1)

plt.show()
