#include "quadratic.h"

Quadratic::Quadratic(double A, double B, double C) : Equation({A, B, C}) {}

std::vector<double> Quadratic::get_x(double y) const{
    std::vector<double> output;

    /* if Quadratic is functionally a line, we can't use the quadratic equation
        formula (divide by zero errors), so we solve it as a line
     */
    double a = coefficients[0], b = coefficients[1], c = coefficients[2];
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

Quadratic Quadratic::operator-(Equation e) const {
    std::vector<double> output = std::vector<double>();
    size_t both = std::min(get_coefficient_count(), e.get_coefficient_count());

    for(size_t i = 0; i < both; i++) {
        output.push_back(coefficients[i] - e.get_coefficient(i));
    }

    for(size_t i = both; i < get_coefficient_count(); i++) {
        output.push_back(coefficients[i]);
    }
    Quadratic q = Quadratic(output[0], output[1], output[2]);
    return q;
}

size_t Quadratic::get_coefficient_count() const {
    return 3;
}
