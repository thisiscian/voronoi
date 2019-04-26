#include "arc.h"

#include <iostream>

Arc::Arc(const Vector2D& v) {
    focus = Vector2D(v);
    left = -std::numeric_limits<double>::infinity();
    right = std::numeric_limits<double>::infinity();
}

double* Arc::get_coefficients(double directrix) const {
    double V1 = focus.x; 
    double V2 = (focus.y + directrix) / 2.0;
    double f = (focus.y - directrix) / 2.0;
    if(f == 0) {
        throw std::domain_error("no value");
    }

    double* output = new double[3] {
        V1 * V1 / (4.0 * f ) + V2,
        -V1 / (2.0 * f),
        1.0 / (4.0 * f)
    };
    return output;
}

Vector2D Arc::at(double x, double directrix) const {
    double* coefficients = get_coefficients(directrix);

    double A = coefficients[0];
    double B = coefficients[1];
    double C = coefficients[2];

    return Vector2D(x, A + B * x +  C * x * x);
}

void Arc::update_intersection(Arc* a, double directrix) {
    double* coeffs, *a_coeffs;
    try {
        coeffs = get_coefficients(directrix);
        a_coeffs = a->get_coefficients(directrix);
    } catch(std::domain_error&) {
        return;
    }
    double _A = coeffs[0] - a_coeffs[0];
    double _B = coeffs[1] - a_coeffs[1];
    double _C = coeffs[2] - a_coeffs[2];

    double det = _B * _B - 4 * _A * _C;
    if(det < 0) {
        throw std::domain_error("Arcs do not intersect");
    }

    double limit[2] {
        (-_B - sqrt(det)) / (2 * _C),  
        (-_B + sqrt(det)) / (2 * _C)
    };

    double x_min = std::min(limit[0], limit[1]);
    double x_max = std::max(limit[0], limit[1]);

    if(left < x_min && x_min <= right) {
        right = a->left = x_min;
        return;
    } 

    if(x_max >= left) {
        right = a->left = x_max;
        return;
    }

    std::cout << "resorted to dubious x_max usage:" << x_max << std::endl;
    std::cout << *this << " X " << *a << ", [" << x_min << ", " << x_max << "] (" << left << ":" << right << ")  " << (x_max - right) << std::endl;
    std::cout << std::endl;
    right = a->left = x_max;
    return;

    throw std::domain_error( "failed to find valid intersection");
}


std::string to_string(const Arc& a){
    std::stringstream ss;
    ss << "Arc(" << a.focus.x << ", " << a.focus.y << " [";
    if(a.left == -std::numeric_limits<double>::infinity()) {
        ss << "-inf";
    } else {
        ss << a.left;
    }
    ss << ", "; 

    if(a.right == std::numeric_limits<double>::infinity()) {
        ss << "inf";
    } else {
        ss << a.right;
    }
    ss << "])";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Arc& a) {
    os << to_string(a);
    return os;
}


bool Arc::operator==(const BeachlineElement& be) {
    try {
        return this == std::get<Arc*>(be);
    } catch(std::bad_variant_access&) {
        return false;
    }
}

bool operator==(const BeachlineElement& be, Arc* arc) {
    try {
        return arc == std::get<Arc*>(be);
    } catch(std::bad_variant_access&) {
        return false;
    }
}

bool operator==(const Event& ev, Arc* arc) {
    try {
        return arc == std::get<Arc*>(ev);
    } catch(std::bad_variant_access&) {
        return false;
    }
}
