#include <algorithm>
#include <float.h>
#include <iostream>
#include "arc.h"

Arc::Arc(Parabola &p): Arc(p, NULL, NULL) {}
Arc::Arc(Parabola &p, Arc* l, Arc* r): parabola(p), left(l), right(r) {
    std::cerr << "UPDATE[NEW ARC]:" << p.focus << "(" << left << ", " << right << ")" << std::endl;
    update(p.focus.get_y());
    std::cerr << "DONE" << std::endl;
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
		double X;
        left->right = this;
        std::vector<double> i, intersections = parabola.get_intersections(left->parabola, directrix);

		if(parabola.focus.get_y() > left->parabola.focus.get_y()) {
			X = left->parabola.focus.get_x();
			std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(i), [X](double x){ return x >= X; });
			left_limit = left->right_limit = *std::min_element(i.begin(), i.end());
		} else {
			X = parabola.focus.get_x();
			std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(i), [X](double x){ return x <= X; });
			left_limit = left->right_limit = *std::max_element(i.begin(), i.end());
		}
    }

    if(right!=NULL) {
		double X;
        right->left = this;
        std::vector<double> i, intersections = parabola.get_intersections(right->parabola, directrix);

		if(parabola.focus.get_y() > right->parabola.focus.get_y()) {
			std::cerr << "RIGHT && above" << std::endl;
			X = right->parabola.focus.get_x();
			std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(i), [X](double x){ return x <= X; });
			right_limit = right->left_limit = *std::min_element(i.begin(), i.end());
		} else {
			X = parabola.focus.get_x();
			std::copy_if(intersections.begin(), intersections.end(), std::back_inserter(i), [X](double x){ return x >= X; });
			right_limit = right->left_limit = *std::max_element(i.begin(), i.end());
		}
    }
    std::cerr << "    update(" << directrix << "): " << *this << std::endl;
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
