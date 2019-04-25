#include "voronoi.h"

#include <iostream>

Voronoi::Voronoi() {}
Voronoi::Voronoi(const std::vector<Vector2D>& points) {
    for(const Vector2D& p: points) {
        if(events.find(p.y) != events.end()) {
            events[p.y].push_back(new Vector2D(p));
        } else {
            events[p.y] = std::vector<Event> { new Vector2D(p) };
        }
    }
};

void Voronoi::solve() {
    while(events.size() > 0) {
        auto event = *events.begin();
        directrix = event.first;
        while(event.second.size() > 0) {
            const auto& v = event.second.back();
            event.second.pop_back();
            beachline.update(directrix);

            std::cout << "Beachline @ " << directrix << std::endl;
            for(const auto& be: beachline.contents) {
                try {
                    Arc* arc = std::get<Arc*>(be);
                    std::cout << "  " << *arc << std::endl;
                } catch(std::bad_variant_access&) {
                }
            }
            std::cout << std::endl;

            try {
                add_arc(std::get<Vector2D*>(v));
            } catch(std::bad_variant_access&) {
                try {
                    remove_arc(std::get<Arc*>(v));
                } catch(std::bad_variant_access&) {}
            }

        }
        events.erase(event.first);
    }

    directrix += 0.1;
    beachline.update(directrix);
    std::cout << "Beachline @ " << directrix << std::endl;
    for(const auto& be: beachline.contents) {
        try {
            Arc* arc = std::get<Arc*>(be);
            std::cout << "  " << *arc << std::endl;
        } catch(std::bad_variant_access&) { }
    }
    return;
};

void Voronoi::add_arc(Vector2D* v) {
    if(beachline.contents.size() == 0) {
        beachline.contents.push_back(new Arc(*v));
        return;
    }
    Arc* parent = beachline.get_arc_above(v);
    remove_event(parent);

    std::vector<BeachlineElement>::iterator it = beachline.replace(parent, v, directrix);
    Arc* l = std::get<Arc*>(it[0]);
    Arc* r = std::get<Arc*>(it[4]);
    check_circle_event(l);
    check_circle_event(r);
}

void Voronoi::remove_arc(Arc* a) {
    Arc* l = beachline.get_arc_on_left(a);
    Arc* r = beachline.get_arc_on_right(a);

    Edge* xl = beachline.get_edge_on_left(a);
    Edge* xr = beachline.get_edge_on_right(a);

    remove_event(l);
    remove_event(r);

    Vector2D s = get_circumcenter(l->focus, a->focus, r->focus);
    Edge* x = new Edge(s, (r->focus - l->focus).get_normal());

    xl->stop = s;
    xr->stop = s;
    complete_edges.push_back(xl);
    complete_edges.push_back(xr);

    beachline.replace(xl, xr, x);

    check_circle_event(l);
    check_circle_event(r);
}

void Voronoi::check_circle_event(Arc* a) {
    try {
        Arc* l = beachline.get_arc_on_left(a);
        Arc* r = beachline.get_arc_on_right(a);

        Edge* xl = beachline.get_edge_on_left(a);
        Edge* xr = beachline.get_edge_on_right(a);

        if(l->focus == r->focus) {
            return;
        }

        Vector2D s = xl->get_intersection(*xr);

        double radius = s.get_distance(a->focus);
        if(s.y + radius < directrix) {
            return;
        }
        std::cout << "new circle event: " << (s.y + radius) << std::endl;
        if(events.find(s.y + radius) != events.end()) {
            events[s.y + radius].push_back(a);
        } else {
            events[s.y + radius] = std::vector<Event> { a };
        }
        
    } catch(std::exception e) {
        return;
    }
}

void Voronoi::remove_event(Arc* a) {
    for(auto& p: events) {
        std::vector<Event>& evs = p.second;
         
        std::vector<Event>::iterator it = std::find(evs.begin(), evs.end(), a);
        if(it != evs.end()) {
            evs.erase(it);
            if(p.second.size() == 0) {
                events.erase(p.first);
                return;
            }

        }

    }
}
