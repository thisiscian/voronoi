#ifndef __voronoi_parabola__
#define __voronoi_parabola__
    
    #include "point.h"
    #include "quadratic.h"

    /* A parabola is, given a focus point and a directrix line, a curve such 
       that for any point P on the parabola, the distance to the focus is equal
       to the tangential distance from P to the directrix
     */
    class Parabola {
        public:
            Parabola(const Point& focus);
            Parabola(const Parabola& p);
            const Point& focus;
            Quadratic to_quadratic(double directrix) const;
            std::vector<double> get_intersections(const Parabola& p, double directrix) const;
            Parabola& operator=(Parabola&& p);
            bool operator==(const Parabola& p) const;
    };

#endif
