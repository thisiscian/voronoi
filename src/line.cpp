#include "line.h"

Line::Line(double A, double B) : Equation({A, B}) {}

std::vector<double> Line::get_x(double y) const {
    if (coefficients[1] == 0) {
        throw std::domain_error("y is constant");
    }
    return std::vector<double>({(y - coefficients[0]) / coefficients[1]});
}

size_t Line::get_coefficient_count() const {
    return 2;
}
