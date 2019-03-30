#ifndef __voronoi_h__
#define __voronoi_h__
    #include "parabola.h"

    template<class T> class Nullable{
        public:
            T& value = NULL;
            T get();

            void operator=(T new_value);
            void clear();
    };

    class Edge {
        public:
            Edge();
            Edge(Point new_start, Point direction);
            Point start;
            Point stop;
            Point direction;
    };

#endif
