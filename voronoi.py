#!/usr/bin/python

import click
import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import pyvoronoi as voronoi

@click.command()
@click.argument("count", default=3)
def main(count):
    fig1 = plt.figure()

    #plt.axis("off")
    axes = plt.gca()
    axes.set_xlim([0,1])
    axes.set_ylim([0,1])
    plt.axis("square")
    plt.axis([0,1,0,1])

    solution = voronoi.solve(count, 0.0)
    scatter = plt.scatter(*solution['points'], marker='.')

    def update(num, beachline, edges, rays, arcs):
        #d = (num / 8000.0 + 0.584652) % 0.59 ## before
        #d = (num / 8000.0 + 0.584653) % 0.59 ## on new point
        #d = (num / 8000.0 + 0.586243) % 0.59 ## just before event
        d = (num / 8000.0 + 0.586244) % 0.59 ## at event

        if d == 0:
            for arc in arcs + edges + rays:
                arc.set_data([], [])

        beachline.set_data([0.0, 1.0], [d, d])
        try:
            solution = voronoi.solve(count, d)

            for i in range(len(solution['edges']) - len(edges) + 1):
                edges.append(plt.plot([],[], '--')[0])

            for i, edge in enumerate(solution['edges']):
                x_data, y_data  = edge
                edges[i].set_data(x_data, y_data)

            for i in range(len(edges), len(solution['edges'])):
                edges[i].set_data([], [])



            for i in range(len(solution['unsolved']) - len(rays)):
                rays.append(plt.plot([],[], '--')[0])

            for i, ray in enumerate(solution['unsolved']):
                x_data, y_data  = ray
                rays[i].set_data(x_data, y_data)

            for i in range(len(solution['unsolved']), len(rays)):
                rays[i].set_data([], [])


            for i in range(len(solution['arcs']) - len(arcs)):
                f, = plt.plot([],[], '-')
                arcs.append(f)

            for i, arc in enumerate(solution['arcs']):
                x, y, left, right = arc
                V1 = x
                V2 = (y + solution['directrix']) / 2.0
                f = (y - solution['directrix']) / 2.0

                A = V1 * V1 / (4.0 * f ) + V2
                B = -V1 / (2.0 * f)
                C = 1.0 / (4.0 * f)

                left = max(0.0, left)
                right = min(1.0, right)
                right = max(left, right)

                x_data = [ i * (right - left )/ 100.0 + left for i in range(101)]
                y_data = [ A + B * x + C * x * x for x in x_data ]
                arcs[i].set_data(x_data, y_data)

            for i in range(len(solution['arcs']), len(arcs)):
                arcs[i].set_data([], [])

            #scatter.set_data(*solution['points'])
            if solution['directrix'] > 1:
                raise StopIteration

        except Exception as e:
            print(e)
            raise e
            pass

        return beachline, 

    edges = []
    rays = []
    arcs = []
    beachline, = plt.plot([], [], '--')

    update(0, beachline, edges, rays, arcs)

    #line_ani = animation.FuncAnimation(fig1, update, fargs = (beachline, edges, rays, arcs))
    plt.show()

if __name__ == '__main__':
    main();
