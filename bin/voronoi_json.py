#!/usr/bin/python

import json
import sys

import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt
import numpy

plt.axis('off')
plt.axis('scaled')

json_string = sys.stdin.read()
json_data = json.loads(json_string)

plot_data=[]

if 'directrix' in json_data:
    d = float(json_data['directrix'])
    plt.axhline(d, -1, 1, linewidth=1, color='k')

if 'points' in json_data:
    x_data = []
    y_data = []

    for point in json_data['points']:
        x_data.append(float(point['x']))
        y_data.append(float(point['y']))
    plot_data += [ x_data, y_data, 'rh' ]
    #plt.plot(x_data, y_data, 'rh',)

if 'border' in json_data:
    x_data = []
    y_data = []

    for point in json_data['border']:
        x_data.append(float(point['x']))
        y_data.append(float(point['y']))

    plot_data += [ x_data, y_data, 'k' ]

if 'unsolved' in json_data:
    for arc in json_data['unsolved']:
        x_data = [float(x) for x in arc['x']]
        y_data = [float(y) for y in arc['y']]

        stop = len(y_data)
        for i, y in enumerate(y_data):
            if y <= 1:
                stop=i+1

        start = 0
        for i, y in enumerate(y_data):
            if y <= 1:
                start = i
                break

        x_data = x_data[start:stop]
        y_data = y_data[start:stop]

        stop = len(x_data)
        for i, x in enumerate(x_data):
            if x <= 1:
                stop=i+1

        start = 0
        for i, x in enumerate(x_data):
            if x <= 1:
                start = i
                break

        x_data = x_data[start:stop]
        y_data = y_data[start:stop]

        plot_data += [ x_data, y_data, 'r' ]

plt.plot(*plot_data,  markersize=2, linewidth=1)
plt.autoscale(axis='both')
plt.show()
