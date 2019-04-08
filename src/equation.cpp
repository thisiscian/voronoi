#include "equation.h"
#include <iostream>

Equation::Equation(std::initializer_list<double> v_args) : coefficients(v_args) {}
Equation::Equation() : Equation({}) {}

const std::string Equation::as_string() const {
    std::stringstream ss;
    ss << "y = "; // << a << " + " << b << "x";
    bool all_zero = true;
    for(size_t i = 0; i < get_coefficient_count(); ++i) {
        if(std::fabs(coefficients[i]) > 1e-300) {
            if (!all_zero) {
                ss << " + ";
            } 
            all_zero = false;
            ss << coefficients[i];
            if (i > 1) {
                ss << "x**" << i; 
            } else if(i == 1) {
                ss << "x";
            }
        }
    }

    if (all_zero) {
        ss << 0;
    }
    
    return ss.str();
}

double Equation::get_y(double x) const {
    double output = 0;
    for(size_t i = 0; i < get_coefficient_count(); ++i) {
        output += coefficients[i] * std::pow(x, i);
    }
    return output;
}

double Equation::get_coefficient(size_t index) const {
    if (index >= get_coefficient_count()) {
        throw std::out_of_range("Supplied index is greater than get_coefficient_count()");
    }
    return coefficients[index];
}

size_t Equation::get_coefficient_count() const {
    return 1;
}

std::vector<double> Equation::get_x(double y) const {
    return std::vector<double>();
}

std::ostream& operator<<(std::ostream& os, const Equation& p) {
	os << p.as_string();
	return os;
}
