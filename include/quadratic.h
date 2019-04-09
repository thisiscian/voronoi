#ifndef __voronoi_quadratic__
#define __voronoi_quadratic__

    #include "equation.h"

    /* Represents a quadratic equation of the form
            y = a + b * x + c * x ** 2
     */
    class Quadratic : public Equation {
        public:
            Quadratic(double A, double B, double C);
            std::vector<double> get_x(double y) const;
            Quadratic operator-(Quadratic e) const;
            size_t get_coefficient_count() const;
    };

#endif
