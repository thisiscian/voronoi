#ifndef __voronoi_h__
#define __voronoi_h__
    #include "parabola.h"
	#include <boost/optional.hpp>
	#include <boost/variant.hpp>
	#include <queue>
	#include <iostream>

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

	class Arc {
		public:
			Arc(Parabola& p);
			Arc(Parabola& p, Arc* l, Arc* r);
			Arc* left;
			Arc* right;
			Parabola& parabola;
	};

	class FortuneOutput {
		public:
			FortuneOutput();
			std::vector<Point> unsolved;
			std::vector<Edge> edges;
	};

	FortuneOutput fortune_solve(std::vector<Point> points);

#endif
