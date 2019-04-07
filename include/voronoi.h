#ifndef __voronoi_h__
#define __voronoi_h__
    #include <boost/optional.hpp>
    #include <boost/variant.hpp>
    #include <queue>
    #include <iostream>

    #include "arc.h"
    #include "point.h"
    #include "parabola.h"

    class Edge {
        public:
            Edge();
            Edge(Point new_start, Point direction);
            Point start;
            boost::optional<Point> stop;
            Point direction;
            const std::string as_string() const;
            bool operator==(const Edge& e) const;
    };

    class FortuneOutput {
        public:
            FortuneOutput();
            std::vector<Arc> unsolved;
            std::vector<Edge> edges;
    };

    FortuneOutput fortune_solve(std::vector<Point> points);

#endif
