#include "arc.h"
#include <iostream>

Arc::Arc(Parabola &p): Arc(p, NULL, NULL) {}
Arc::Arc(Parabola &p, Arc* l, Arc* r): parabola(p), left(l), right(r) {
	update(p.focus.get_y());
}

std::string Arc::as_string() const {
    std::stringstream ss;
    ss << "Arc(focus=" << parabola.focus << ", limits=[";
    if (left != NULL) {
        ss << left_limit;
    } else {
        ss << "-∞";
    }
    ss << ", ";
    if (right != NULL) {
        ss << right_limit;
    } else {
        ss << "∞";
    }
    ss << "])";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Arc& a) {
    os << a.as_string();
    return os;
}

void Arc::update(double directrix) {
    if(left!=NULL) {
        left->right = this;
        std::vector<Point> left_intersections = parabola.get_intersections(left->parabola, directrix);
        if(left_intersections.size() > 0) {
			std::cerr << "lefters(" << parabola.focus << ") " << std::endl;
			for(const Point& x: left_intersections) {
				std::cerr << " --> " << x << std::endl;
			}
            left_limit = left->right_limit = left_intersections.back().get_x();
        }
    }

    if(right!=NULL) {
        right->left = this;
        std::vector<Point> right_intersections = parabola.get_intersections(right->parabola, directrix);
        if(right_intersections.size() > 0) {
			std::cerr << "righters(" << parabola.focus << ") " << std::endl;
			for(const Point& x: right_intersections) {
				std::cerr << " --> " << x << std::endl;
			}
            right_limit = right->left_limit = right_intersections.front().get_x();
        }
    }
}

bool Arc::underneath(Point &p) const {
    if(left != NULL && p.get_x() < left_limit) {
        return false;
    }

    if(right != NULL && p.get_x() > right_limit) {
        return false;
    }

    return true; 
}

bool Arc::on_left(Point &p) const {
    return p.get_x() < parabola.focus.get_x();
}


