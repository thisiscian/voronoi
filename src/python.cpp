#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <iostream>
#include <list>
#include <limits>
#include <cstdlib>

#include "vector2d.h"
#include "voronoi.h"

using namespace boost::python;

dict py_solve(size_t count, double goal_directrix = DBL_MAX) {
    srand(10);
    dict output;
    list unsolved, edges, output_points, arcs;
    output["points"] = output_points;
    output["unsolved"] = unsolved;
    output["edges"] = edges;
    output["arcs"] = arcs;
    std::cout << "goal_directrix: " << goal_directrix << std::endl;

    list x_data, y_data;
    output_points.append(x_data);
    output_points.append(y_data);
    std::vector<Vector2D> points;
    for(size_t i = 0; i < count; ++i) {
        Vector2D v = Vector2D::get_random(0, 1);
        //std::cout << v << std::endl;
        points.push_back(v);
        x_data.append(points.back().x);
        y_data.append(points.back().y);
    }

    Voronoi voronoi(points);
    voronoi.solve(goal_directrix);
    output["directrix"] = voronoi.directrix;

    for(Edge* e: voronoi.complete_edges) {
        list edge, x_data, y_data;
        
        edges.append(edge);
        edge.append(x_data);
        edge.append(y_data);
        x_data.append(e->start.x);
        y_data.append(e->start.y);
        Vector2D d = *e->stop;
        x_data.append(d.x);
        y_data.append(d.y);
    }

    for(BeachlineElement& be: voronoi.beachline.contents) {
        try {
            Edge* e = std::get<Edge*>(be);
            list edge, x_data, y_data;
            unsolved.append(edge);
            edge.append(x_data);
            edge.append(y_data);
            x_data.append(e->start.x);
            y_data.append(e->start.y);
            Vector2D d = e->start + 3 * e->direction;
            x_data.append(d.x);
            y_data.append(d.y);
        } catch(std::bad_variant_access&) {
            Arc* a = std::get<Arc*>(be);
            list arc;
            arc.append(a->focus.x);
            arc.append(a->focus.y);
            arc.append(a->left);
            arc.append(a->right);
            arcs.append(arc);
        }
    }

    return output;
}

BOOST_PYTHON_MODULE(pyvoronoi) {
    using namespace boost::python;

    def("solve", py_solve);
}
