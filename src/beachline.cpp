#include "beachline.h"

#include <iostream>

Arc* Beachline::get_arc_above(Vector2D* v) const {
    for(const auto& content: contents ) {
        try {
            Arc* arc = std::get<Arc*>(content);
            if(arc->right >= v->x && arc->left <= v->x) {
                return arc;
            }
        } catch(std::bad_variant_access&) {}
    }
    throw std::domain_error("Could not find arc above " + to_string(*v));
}

void Beachline::update(double directrix) {
    auto end = prev(prev(contents.end()));
    size_t c = contents.end() - contents.begin();
    for(auto it = contents.begin(); it != contents.end(); it+=2) {
        size_t q = it - contents.begin();
        if(q + 2 >= c)
            break;

        try {
            Arc* a = std::get<Arc*>(*(it + 2));
            std::get<Arc*>(*it)->update_intersection(a, directrix);
        } catch(std::bad_variant_access&){
        }
    }
};

std::vector<BeachlineElement>::iterator Beachline::replace(Arc* parent, Vector2D* new_focus, double directrix) {
    Vector2D p = parent->at(new_focus->x, directrix);

    Vector2D normal = (*new_focus - parent->focus).get_normal(); 
    if(normal.x > 0) {
        normal = -1.0 * normal;
    }

    std::vector<BeachlineElement> new_inserts {
        new Arc(parent->focus),
        new Edge(p, normal),
        new Arc(*new_focus),
        new Edge(p, -1.0 * normal),
        new Arc(parent->focus)
    };
    std::get<Arc*>(new_inserts[0])->left = parent->left;
    std::get<Arc*>(new_inserts[0])->right = p.x;
    std::get<Arc*>(new_inserts[2])->left = p.x;
    std::get<Arc*>(new_inserts[2])->right = p.x;
    std::get<Arc*>(new_inserts[4])->left = p.x;
    std::get<Arc*>(new_inserts[4])->right = parent->right;

    auto it = std::find(contents.begin(), contents.end(), parent);
    it = contents.erase(it);
    return contents.insert(it, new_inserts.begin(), new_inserts.end());
}

std::vector<BeachlineElement>::iterator Beachline::replace(Edge* xl, Edge* xr, Edge* X) {
    auto start_it = std::find(contents.begin(), contents.end(), xl);
    auto stop_it = std::find(contents.begin(), contents.end(), xr);
    auto it = contents.erase(start_it, stop_it + 1);
    return contents.insert(it, X);
}

Arc* Beachline::get_arc_on_left(Arc* a) const {
    auto it = std::find(contents.begin(), contents.end(), a);
    if(it == contents.end()) {
        throw std::domain_error("Could not find Arc in beachline:" + to_string(*a));
    } else if(it == contents.begin()) {
        throw std::domain_error("Arc is leftmost arc");
    } else {
        return std::get<Arc*>(it[-2]);
    }
};

Arc* Beachline::get_arc_on_right(Arc* a) const {
    auto it = std::find(contents.begin(), contents.end(), a);
    if(it == contents.end()) {
        throw std::domain_error("Arc is rightmost arc");
    } else {
        return std::get<Arc*>(it[2]);
    }
};

Edge* Beachline::get_edge_on_left(Arc* a) const {
    auto it = std::find(contents.begin(), contents.end(), a);
    if(it == contents.begin()) {
        throw std::domain_error("Arc is leftmost arc");
    } else {
        return std::get<Edge*>(it[-1]);
    }
};

Edge* Beachline::get_edge_on_right(Arc* a) const {
    auto it = std::find(contents.begin(), contents.end(), a);
    if(it == contents.end()) {
        throw std::domain_error("Arc is rightmost arc");
    } else {
        return std::get<Edge*>(it[1]);
    }
};
