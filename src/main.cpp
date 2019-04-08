#include <cstdlib>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "point.h"
#include "arc.h"
#include "voronoi.h"

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

std::vector<Point> generate_points_in_rect(int num_points, double x=0.0, double y=0.0, double width=1.0, double height=1.0) {
    std::vector<Point> output; 
    for(int i=0; i<num_points; ++i) {
        double X = (double)rand() * width / RAND_MAX + x;
        double Y = (double)rand() * width / RAND_MAX + x;
        output.push_back(Point(X, Y));
    }
    return output;
}

std::vector<Point> generate_points_in_parameterised_function(int num_points, double* (*parameter_func)(double t) ) {
    std::vector<Point> output; 
	size_t redo_count = 0;
    for(int i=0; i<num_points; ++i) {

		double t = ((double)rand()) / RAND_MAX;
		double r = ((double)rand()) / RAND_MAX;
		double *V = parameter_func(t);
		Point p(r * V[0], r * V[1]);

		bool do_skip = false;
		for(const Point& P: output) {
			if(p.distance_to(P) < 1e-1) {
				do_skip = true;
				break;
			}
		}

		if(do_skip) {
			redo_count++;
			--i;
			if(redo_count > 100) {
				break;
			} else {
				continue;
			}
		}

        output.push_back(p);
		redo_count = 0;
    }
    return output;
}

double* heart(double t) {
	double T = t * 2 * M_PI;
	double x = 16 * pow(sin(T), 3);
	double y = 13 * cos(T) - 5 * cos(2 * T) - 2 * cos(3 * T) - cos(4 * T);
	
	double *output = new double[2]{x / 16.0, y / 16.0};
	return output;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

	ptree pt;
	pt.put("version", "0.01");

	ptree border;
	for(double t=0.0; t<=1.0; t+= 1e-2) {
		double *p = heart(t);
		ptree child;
		child.put("x", p[0]);
		child.put("y", p[1]);
		border.push_back(std::make_pair("", child));
	}
	pt.add_child("border", border);

	std::vector<Point> points = generate_points_in_parameterised_function(3, heart);
	ptree voronoi_points;
	double directrix = 100;
    for(const Point& p: points) {
		ptree child;
		child.put("x", p.get_x());
		child.put("y", p.get_y());

		if(p.get_y() < directrix)
			directrix = p.get_y();

		voronoi_points.push_back(std::make_pair("", child));
    }
	pt.add_child("points", voronoi_points);
	std::cerr << "DIRECTRIX:" << directrix << std::endl;

	FortuneOutput output = fortune_solve(points);
	pt.put("directrix", directrix);
	ptree unsolved;
	for(Arc& a: output.unsolved) {
		a.update(directrix);
		std::cerr << a << " " << directrix << std::endl;

		double left, right;
		if(a.left) {
			left = a.left_limit;
		} else {
			left = -1.0;
		}

		if(a.right) {
			right = a.right_limit;
		} else {
			right = -1.0;
		}
		
		if(!(left <= right)) {
			continue;
		}

		ptree child, x_array, y_array;

		if(left == right) {
			ptree X, Y;
			X.put("", a.parabola.focus.get_x());
			X.put("", a.parabola.focus.get_x());
			Y.put("", -1.0);
			Y.put("", a.parabola.focus.get_y());
			x_array.push_back(std::make_pair("", X));
			y_array.push_back(std::make_pair("", Y));

		} else {
			for(double x = left; x <= right; x += 1e-2) {
				Quadratic q = a.parabola.to_quadratic(-2.0);
				double y = q.get_y(x);
				ptree X, Y;
				X.put("", x);
				Y.put("", y);
				x_array.push_back(std::make_pair("", X));
				y_array.push_back(std::make_pair("", Y));
			}
		}
		child.add_child("x", x_array);
		child.add_child("y", y_array);
		unsolved.push_back(std::make_pair("", child));
	}
	pt.add_child("unsolved", unsolved);

	std::ostringstream buf;
	write_json(std::cout, pt, false);
}
