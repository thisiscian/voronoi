#ifndef __voronoi_equation__
#define __voronoi_equation__

    #include <initializer_list>
    #include <stdexcept>
    #include <vector>
    #include <cmath>

    #include "base.h"

    /* Represents an equation of the form
            y = a + b * x + c * x ** 2 + d * x ** 3 + ...
     */
    class Equation {
        public:
            Equation(std::initializer_list<double> v_args);
            Equation();
            const std::string as_string() const;

            double get_coefficient(size_t index) const;
            virtual size_t get_coefficient_count() const;

            double get_y(double x) const;
            virtual std::vector<double> get_x(double y) const;

            friend std::ostream& operator<<(std::ostream&, const Equation& p);
        protected:
            std::vector<double> coefficients;
    };

#endif
