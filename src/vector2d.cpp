#include "vector2d.h"

Vector2D::Vector2D(): x(0), y(0) {};
Vector2D::Vector2D(double X, double Y): x(X), y(Y) {};
Vector2D::Vector2D(const Vector2D& p): x(p.x), y(p.y) {};

Vector2D Vector2D::get_normal() const {
    double l = get_length();
    if( l == 0 ){
        return Vector2D(0, 0);
    } else {
        return Vector2D(- y / l, x / l);
    }
}

Vector2D Vector2D::get_unit_vector() const {
    double l = get_length();
    if( l == 0 ){
        return Vector2D(0, 0);
    } else {
        return Vector2D(x / l, y / l);
    }
}

double Vector2D::get_distance(const Vector2D& v) const {
    return sqrt(pow(x - v.x, 2) + pow(y - v.y, 2));
}

double Vector2D::get_length() const {
    return sqrt(pow(x, 2) + pow(y, 2));
}

double Vector2D::get_sq_length() const {
    return pow(x, 2) + pow(y, 2);
}

bool Vector2D::operator==(const Vector2D& v) const {
    return x == v.x && y == v.y;
}

Vector2D Vector2D::operator+(const Vector2D& v) const {
    return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D& v) const {
    return Vector2D(x - v.x, y - v.y);
}

bool Vector2D::operator<(const Vector2D& v) const {
    return y < v.y || (y == v.y && x < v.x);
}

Vector2D operator*(double a, const Vector2D& v) {
    return Vector2D(a * v.x, a * v.y);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << to_string(v);
    return os;
}

std::string to_string(const Vector2D& v){
    std::stringstream ss;
    ss << "Vector2D(" << v.x << ", " << v.y << ")";
    return ss.str();
}

Vector2D Vector2D::get_random(double min = 0, double max = 1) {
    return Vector2D(rand() * 1.0 / RAND_MAX + min, rand() * (max - min) / RAND_MAX + min);
}

Vector2D get_circumcenter(const Vector2D& a, const Vector2D& b, const Vector2D& c) {
    Vector2D m_ab = 0.5 * (a + b);
    Vector2D m_bc = 0.5 * (b + c);

    Vector2D ab = b - a;
    Vector2D bc = c - b;

    Vector2D B = b - a;
    Vector2D C = c - a;

    double d = 2 * (B.x * C.y - B.y * C.x);
    if(d == 0) {
        throw std::domain_error("There is no circumcenter");
    }
    double x = (C.y * B.get_sq_length() - B.y * C.get_sq_length()) / d;
    double y = (B.x * C.get_sq_length() - C.x * B.get_sq_length()) / d;
    return Vector2D(x, y) + a;
}
