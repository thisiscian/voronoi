#ifndef __voronoi_arc_h__
#define __voronoi_arc_h__
    #include <exception>
    #include <limits>
    #include <ostream>
    #include <sstream>
    #include <string>
    #include <variant>

    #include "beachline.h"
    #include "edge.h"
    #include "vector2d.h"

    class Arc;

    typedef std::variant<Arc*, Edge*> BeachlineElement;
    typedef std::variant<Vector2D*, Arc*> Event;

    class Arc {
        public:
            Arc(const Vector2D&);

            Vector2D focus;
            double left, right;

            double* get_coefficients(double) const;
            Vector2D at(double, double) const;
            void update_intersection(Arc*, double);

            bool operator==(const BeachlineElement&);

            friend std::ostream& operator<<(std::ostream&, const Arc&);
            friend std::string to_string(const Arc&);
            friend bool operator==(const BeachlineElement& be, Arc* arc);
            friend bool operator==(const Event&, Arc*);
    };

#endif
