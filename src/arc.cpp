#include "arc.h"

#include <iostream>

Arc::Arc(const Vector2D& v) {
    focus = Vector2D(v);
    left = -std::numeric_limits<double>::max();
    right = std::numeric_limits<double>::max();
}

Vector2D Arc::at(double x, double directrix) const {
    double V1 = focus.x; 
    double V2 = (focus.y + directrix) / 2.0;
    double f = (focus.y - directrix) / 2.0;

    double A = V1 * V1 / (4.0 * f ) + V2;
    double B = -V1 / (2.0 * f);
    double C = 1.0 / (4.0 * f);

    return Vector2D(x, A + B * x +  C * x * x);
}

void Arc::update_intersection(Arc* a, double directrix) {
    double V1 = focus.x; 
    double V2 = (focus.y + directrix) / 2.0;
    double f = (focus.y - directrix) / 2.0;
    
    if(f == 0) {
        return;
    }

    double A = V1 * V1 / (4.0 * f ) + V2;
    double B = -V1 / (2.0 * f);
    double C = 1.0 / (4.0 * f);

    double aV1 = a->focus.x; 
    double aV2 = (a->focus.y + directrix) / 2.0;
    double af = (a->focus.y - directrix) / 2.0;

    if(af == 0) {
        return;
    }

    double aA = aV1 * aV1 / (4.0 * af ) + aV2;
    double aB = -aV1 / (2.0 * af);
    double aC = 1.0 / (4.0 * af);

    double _A = A - aA;
    double _B = B - aB;
    double _C = C - aC;

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
    //std::cout << "    \x1b[1m" << x_min << ", " << x_max << "\x1b[0m | " << left << " " << right<< std::endl;

    if(left < x_min && x_min <= right) {
        right = a->left = x_min;
        return;
    } 

    if(x_max >= right) {
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
    if(a.left == -std::numeric_limits<double>::max()) {
        ss << "-inf";
    } else {
        ss << a.left;
    }
    ss << ", "; 

    if(a.right == std::numeric_limits<double>::max()) {
        ss << "inf";
    } else {
        ss << a.right;
    }
    ss << "])";
    return ss.str();
}

std::string to_string(const Arc& a, double directrix) {
    std::stringstream ss;
    double V1 = a.focus.x; 
    double V2 = (a.focus.y + directrix) / 2.0;
    double f = (a.focus.y - directrix) / 2.0;

    double A = V1 * V1 / (4.0 * f ) + V2;
    double B = -V1 / (2.0 * f);
    double C = 1.0 / (4.0 * f);

    ss << A << "+" << B << "x+" << C << "x^2";
    if(a.left > -std::numeric_limits<double>::max() && a.right == std::numeric_limits<double>::max()) {
        ss << " \\left\\{ x > " << a.left << "\\right\\}";
    } else if(a.left > -std::numeric_limits<double>::max() || a.right < std::numeric_limits<double>::max()) {
        ss << " \\left\\{";
        if(a.left > -std::numeric_limits<double>::max()) {
            ss << a.left << " < ";
        }
        ss << " x"; 

        if(a.right < std::numeric_limits<double>::max()) {
            ss << " < " << a.right;
        }
        ss << "\\right\\}";
    }
    
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
