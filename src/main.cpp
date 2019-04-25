#include <cstdlib>
#include <iostream>

#include "vector2d.h"
#include "voronoi.h"

int main(int argc, char* argv[]) {
    srand(10);

    int count = 4;
    if(argc > 1) {
        count = atoi(argv[1]);
    }

    std::vector<Vector2D> points;
    std::cout << "Vector2Ds:" << std::endl;
    for(int i=0; i<count; ++i) {
        Vector2D v = Vector2D::get_random(0, 1);
        points.push_back(v);
        std::cout << "  " << v << std::endl;
    }
    std::cout << std::endl;

    Voronoi voronoi(points);
    voronoi.solve();

    for(Edge* e: voronoi.complete_edges) {
        std::cout << "complete: " << e->start << "<->" << *e->stop << std::endl;
    }

    for(BeachlineElement& be: voronoi.beachline.contents) {
        try {
            Edge* e = std::get<Edge*>(be);
            std::cout << "incomplete: " << e->start << "<->" << (e->start + 10.0 * e->direction) << std::endl;
        } catch(std::bad_variant_access&) {}
    }

}
