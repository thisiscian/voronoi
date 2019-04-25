#ifndef __voronoi_beachline_h__
#define __voronoi_beachline_h__
    #include <algorithm>
    #include <variant>
    #include <vector>
    #include "arc.h"
    #include "edge.h"
    #include "vector2d.h"

    class Arc;
    class Edge;
    typedef std::variant<Arc*, Edge*> BeachlineElement;
    
    class Beachline {
        public:
            std::vector<BeachlineElement> contents;

            void update(double);
            Arc* get_arc_above(Vector2D*) const;
            std::vector<BeachlineElement>::iterator replace(Arc*, Vector2D*, double);
            std::vector<BeachlineElement>::iterator replace(Edge*, Edge*, Edge*);

            Arc* get_arc_on_left(Arc*) const;
            Arc* get_arc_on_right(Arc*) const;

            Edge* get_edge_on_left(Arc*) const;
            Edge* get_edge_on_right(Arc*) const;
    };
#endif
