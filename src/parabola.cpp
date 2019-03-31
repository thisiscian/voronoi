#include "parabola.h"

Point::Point() {
    x = 0.0;
    y = 0.0;
}

Point::Point(double X, double Y) {
    x = X;
    y = Y;
}

const std::string Point::as_string() const {
    std::stringstream ss;
    ss << "Point(" << x << ", " << y << ")";
    return ss.str();
}

bool Point::operator==(const Point& p) const {
    return x == p.x && y == p.y;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << p.as_string();
    return os;
}

Line::Line() {
    a = 0;
    b = 0;
}

Line::Line(double A, double B) {
    a = A;
    b = B;
}

const std::string Line::as_string() const {
    std::stringstream ss;
    ss << "y = " << a << " + " << b << "x";
    return ss.str();
}

double Line::get_y(double x) const {
    return b * x + a;
}

double Line::get_x(double y) const {
    if (b == 0) {
        throw std::domain_error("y is constant");
    }
    return (y - a) / b;
}

Quadratic::Quadratic(double A, double B, double C) {
    a = A;
    b = B;
    c = C;
}

const std::string Quadratic::as_string() const {
    std::stringstream ss;
    ss << "y = " << a << " + " << b << "x + " << c << "x²";
    return ss.str();
}

double Quadratic::get_y(double x) const {
    return c * x * x + b * x + a;
}

std::vector<double> Quadratic::get_x(double y) const{
    std::vector<double> output;

    /* if Quadratic is functionally a line, we can't use the quadratic equation
        formula (divide by zero errors), so we solve it as a line
     */
    if (c == 0) {
        if (b == 0) {
            throw std::domain_error("y is constant");
        }
        output.push_back((y - a) / b);
        return output;
    } 

    double determinant = b * b - 4.0 * c * (a - y);
    if (determinant < 0) {
        throw std::domain_error("only imaginary values of x at this y");
    } else if (determinant == 0) {
        output.push_back(-b / (2.0 * c));
    } else {
        double sq_d = sqrt(determinant);
        output.push_back((-b + sq_d) / (2.0 * c));
        output.push_back((-b - sq_d) / (2.0 * c));
    }

    return output;
}

Quadratic Quadratic::operator-(const Quadratic& q) const {
    return Quadratic(
        a - q.a,
        b - q.b,
        c - q.c
    );
}

std::ostream& operator<<(std::ostream& os, const Quadratic& q) {
    os << q.as_string();
    return os;
}

Parabola::Parabola(Point new_focus) {
    focus = new_focus;
}

Parabola::Parabola(double X, double Y) {
    focus = Point(X, Y);
}

Quadratic Parabola::to_quadratic(double directrix) const {
    double V1 = focus.x;
    double V2 = (focus.y - directrix) / 2.0;
    double f = focus.y - directrix;

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

    for(const double& x: (q1 - q2).get_x(0.0)) {
        output.push_back(Point(x, q1.get_y(x)));
    }

    return output;
};
