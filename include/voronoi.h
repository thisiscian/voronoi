#ifndef __voronoi_voronoi_h__
#define __voronoi_voronoi_h__
    #include <algorithm>
    #include <exception>
    #include <limits>
    #include <map>
    #include <variant>
    #include <vector>
    #include "arc.h"
    #include "beachline.h"
    #include "edge.h"
    #include "vector2d.h"

    typedef std::variant<Vector2D*, Arc*> Event;

    class Voronoi {
        public:
            Voronoi();
            Voronoi(const std::vector<Vector2D>&);

            Beachline beachline;
            double directrix;
            std::map<double, std::vector<Event> > events;
            std::vector<Edge*> complete_edges;

            void solve(double x = DBL_MAX);
            void add_arc(Vector2D*);
            void remove_arc(Arc*);
            void remove_event(Arc*);
            void check_circle_event(Arc*);
    };
#endif
