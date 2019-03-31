#!/usr/bin/python

import pyvoronoi as v

point1 = v.Point(1, 0)
point2 = v.Point(2, 0)
print(point1)
print(point1.x)
print()

line1 = v.Line(1, 1)
print(line1)
print(line1.a, line1.b)
print(line1.get_y(0), line1.get_x(0))
print()

quadratic1 = v.Quadratic(0, 0, 1)
print(quadratic1)
print(quadratic1.a, quadratic1.b, quadratic1.c)
xs = quadratic1.get_x(0)
print(quadratic1.get_y(0), xs)
for i in xs:
    print("    ", i)
xs = quadratic1.get_x(1)
for i in xs:
    print("    ", i)
print()


parabola1 = v.Parabola(point1)
parabola2 = v.Parabola(point2)
print("focus", parabola1.focus)
print()

q1 = parabola1.to_quadratic(0)
q2 = parabola2.to_quadratic(0)

print(q1, q1.a, q1.b, q1.c)
print(q2, q2.a, q2.b, q2.c)

print(parabola1.get_intersections(parabola2, 0));
print(v.solve([point1, point2]))
