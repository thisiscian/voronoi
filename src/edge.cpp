#include "edge.h"

Edge::Edge() : start(Vector2D()), direction(Vector2D()) {}
Edge::Edge(const Vector2D& S, const Vector2D& D) : start(S), direction(D) {}

bool Edge::operator==(const BeachlineElement& be) {
    try {
        return this == std::get<Edge*>(be);
    } catch(std::bad_variant_access&) {
        return false;
    }
}

bool Edge::operator==(const Edge& e) {
    return this == &e;
}

Vector2D Edge::get_intersection(const Edge& e) const {
    if(direction.x == 0 && e.direction.x == 0) {
        throw std::domain_error("Lines are parallel");
    }

    //y = ax + c
    double a = direction.y / direction.x;
    double c = start.y - a * start.x;

    // y = bx + d
    double b = e.direction.y / e.direction.x;
    double d = e.start.y - b * e.start.x;

    if(a == b) {
        throw std::domain_error("Lines are parallel");
    }

    return Vector2D((d - c) / (a - b), (a * d - b * c) / (a - b));
}

bool operator==(const BeachlineElement& be, Edge* edge) {
    try {
        return edge == std::get<Edge*>(be);
    } catch(std::bad_variant_access&) {
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const Edge& e) {
    os << to_string(e);
    return os;
}

std::string to_string(const Edge& e) { 
    std::stringstream ss;
    ss << "Edge(" << e.start.x << ", " << e.start.y << " -> " << e.direction.x << ", " << e.direction.y << ")";
    return ss.str();
}
