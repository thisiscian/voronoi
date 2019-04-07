#ifndef __voronoi_line__
#define __voronoi_line__

    #include "equation.h"

    /* Represents a quadratic equation of the form
            y = a + b * x
     */
    class Line : public Equation {
        public:
            Line(double A, double B);
            std::vector<double> get_x(double y) const;
            size_t get_coefficient_count() const;
    };

#endif
