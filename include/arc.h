#ifndef __voronoi_arc__
#define __voronoi_arc__
    #include <string>
    #include <sstream>

    #include "point.h"
    #include "parabola.h"


    class Arc {
        public:
            Arc(Parabola& p);
            Arc(Parabola& p, Arc *l, Arc *r);
            Parabola& parabola;
            Arc *left, *right;
            double left_limit, right_limit;

            std::string as_string() const;
            friend std::ostream& operator<<(std::ostream&, const Arc& a);

            bool underneath(Point& p) const;
            bool on_left(Point& p) const;
            void update(double directrix);
    };

#endif
