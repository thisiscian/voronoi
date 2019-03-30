#include <iostream>
#include "parabola.h"

#include <cstdlib>

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

    Line line(1.0, 1.0);
    double line_values[] = {0.0, 1.0};
    double line_solutions[] = {1.0, 2.0};

    std::cout << line.as_string() << std::endl;
    for(const double& x: line_values) {
        std::cout << "    x = " << x << " => y = " << line.get_y(x) << std::endl;
    }
    for(const double& y: line_solutions) {
        std::cout << "    y = " << y << " => x = " << line.get_x(y) << std::endl;
    }
    std::cout << std::endl;

    Quadratic quadratic(0.0, 0.0, 1.0);
    std::cout << quadratic.as_string() << std::endl;
    double quadratic_values[] = {-1.0, 0.0, 1.0};
    double quadratic_solutions[] = {0.0, 1.0};
    for(const double& x: quadratic_values) {
        std::cout << "    x = " << x << " => y = " << quadratic.get_y(x) << std::endl;
    }
    for(const double& y: quadratic_solutions) {
        std::vector<double> xs = quadratic.get_x(y);
        std::cout << "    y = " << y << " => y = [";
        for(const double& x: xs) {
            std::cout << x << ", ";
        }
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;

    Parabola parabola1(Point(0.0, 10.0));
    Parabola parabola2(Point(10.0, 5.0));
    Quadratic qp1 = parabola1.to_quadratic(0.0);
    Quadratic qp2 = parabola2.to_quadratic(0.0);
    Quadratic qp3 = qp1 - qp2;
    std::cout << qp1 << std::endl;
    std::cout << qp2 << std::endl;
    std::cout << qp3 << std::endl;

    std::cout << "    solutions = [";
    for(const double& x: qp3.get_x(0.0)) {
        std::cout << "(" << x << ", " << qp1.get_y(x) << "), ";
    }
    std::cout << "]" << std::endl;

    std::cout << " or solutions = [";
    for(const Point& p: parabola1.get_intersections(parabola2, 0.0)) {
        std::cout << p << ", ";
    }
    std::cout << "]" << std::endl;

    std::cout << std::endl;
    std::cout << "random_points => ";
    for(const Point& p: generate_points_in_rect(2)) {
        std::cout << p << ", ";
    }
    std::cout << std::endl;
}
