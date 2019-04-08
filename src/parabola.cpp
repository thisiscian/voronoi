#include "parabola.h"

#include <iostream>

Parabola::Parabola(const Parabola& p) : focus(p.focus) {}
Parabola::Parabola(const Point& new_focus) : focus(new_focus) {}

Quadratic Parabola::to_quadratic(double directrix) const {
    double V1 = focus.get_x();
    double V2 = (focus.get_y() + directrix) / 2.0;
    double f = (focus.get_y() - directrix) / 2.0;

    return Quadratic(
        V1 * V1 / (4.0 * f) + V2,
        -V1 / (2.0 * f),
        1.0 / (4.0 * f)
    );
}

std::vector<Point> Parabola::get_intersections(const Parabola& p, double directrix) const {
    const Quadratic q1 = to_quadratic(directrix);
    const Quadratic q2 = p.to_quadratic(directrix);

    std::vector<Point> output;
	if(directrix == p.focus.get_y()) {
		double x = p.focus.get_x();
		double y = q1.get_y(x);

		output.push_back(Point(x, y));
		output.push_back(Point(x, y));
		return output;
	}

	if(directrix == focus.get_y()) {
		double x = focus.get_x();
		double y = q2.get_y(x);

		output.push_back(Point(x, y));
		output.push_back(Point(x, y));
		return output;
	}

    for(const double& x: (q1 - q2).get_x(0.0)) {
        output.push_back(Point(x, 0.0));
    }

    return output;
};

Parabola& Parabola::operator=(Parabola&& p) {
	return p;
}

bool Parabola::operator==(const Parabola& p) const {
	return p.focus == focus;
}


