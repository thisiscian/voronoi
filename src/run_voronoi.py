#!/usr/bin/python

import sys
import math
import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt
import pyvoronoi as v

def parameterised_heart(t):
	T = t * 2 * math.pi;
	x = 16 * pow(math.sin(T), 3)
	y = 13 * math.cos(T) - 5 * math.cos(2 * T) - 2 * math.cos(3 * T) - math.cos(4 * T)
	return [x / 16.0, y / 16.0]

data = [parameterised_heart(t/100.0) for t in range(0, 100)]
xdata = [d[0] for d in data]
ydata = [d[1] for d in data]

plt.ylabel("y")
plt.xlabel("x")
plt.title("a heart")
plt.plot(xdata, ydata)
plt.show()

parabola1 = v.Parabola(v.Point(0,1))
parabola2 = v.Parabola(v.Point(0,2))
print("focus", parabola1.focus)
print()

q1 = parabola1.to_quadratic(0)
q2 = parabola2.to_quadratic(0)

print(q1, q1.a, q1.b, q1.c)
print(q2, q2.a, q2.b, q2.c)

print("intersections:")
for i in parabola1.get_intersections(parabola2, 0):
    print("    ", i)

#print("solve():")
#print(v.solve([point1, point2]))
