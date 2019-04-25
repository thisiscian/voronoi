#!/usr/bin/python

import click
import matplotlib
matplotlib.use('GTK3Agg')
import matplotlib.pyplot as plt

import pyvoronoi as voronoi

@click.command()
@click.argument("count", default=3)
def main(count):
    solution = voronoi.solve(count)

    for x_data, y_data in solution['edges']:
        plt.plot(x_data, y_data, '-')

    for x_data, y_data in solution['unsolved']:
        plt.plot(x_data, y_data, '--')

    plt.scatter(*solution['points'], marker=".")

    plt.axis("off")
    plt.axis("equal")
    plt.axis([0,1,0,1])
    plt.show()

if __name__ == '__main__':
    main();
