#include <cstdlib>
#include <iostream>

#include "point.h"
#include "line.h"
#include "quadratic.h"
#include "parabola.h"
#include "voronoi.h"


std::vector<Point> generate_points_in_rect(int num_points, double x=0.0, double y=0.0, double width=1.0, double height=1.0) {
    std::vector<Point> output; 
    for(int i=0; i<num_points; ++i) {
        double X = (double)rand() * width / RAND_MAX + x;
        double Y = (double)rand() * width / RAND_MAX + x;
        output.push_back(Point(X, Y));
    }
    return output;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    std::cout << "random_points => ";
    for(const Point& p: generate_points_in_rect(2)) {
        std::cout << p << ", ";
    }
    std::cout << std::endl;
}
