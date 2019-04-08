#include "point.h"

Point::Point(double X, double Y) {
    x = X;
    y = Y;
}
Point::Point() : Point(0.0, 0.0) {};

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

double Point::get_x() const {
    return x;
}

void Point::set_x(double value) {
    x = value;
}

double Point::get_y() const {
    return y;
}

void Point::set_y(double value) {
    y = value;
}

double Point::distance_to(const Point& p) const {
	return sqrt(std::pow(x - p.get_x(), 2) + std::pow(y - p.get_y(), 2));
}

double Point::length() const {
	return sqrt( std::pow(x, 2) + std::pow(y, 2));
}
