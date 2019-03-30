#ifndef __parabola_h__
#define __parabola_h__
    #include <stdexcept>
    #include <vector>
    #include <cmath>
    #include <string>
    #include <sstream>
    #include <ostream>

    class Point {
        public:
            Point();
            Point(double X, double Y);
            double x, y;
            friend std::ostream& operator<<(std::ostream&, const Point& p);
    };

    /* Represents a quadratic equation of the form
            y = b * x + a
     */
    class Line {
        public:
            Line();
            Line(double A, double B);
            const std::string as_string() const;
            double a, b; 
            double get_y(double x) const;
            double get_x(double y) const;
    };

    /* Represents a quadratic equation of the form
           y = c * x * x + b * x + a
     */
    class Quadratic {
        public:
            Quadratic(double A, double B, double C);
            const std::string as_string() const;
            double a, b, c;
            double get_y(double x) const;
            std::vector<double> get_x(double y) const;
            Quadratic operator-(const Quadratic& q) const;
            friend std::ostream& operator<<(std::ostream& os, const Quadratic& q);
    };


    /* A parabola is, given a focus point and a directrix line, a curve such 
       that for any point P on the parabola, the distance to the focus is equal
       to the tangential distance from P to the directrix
     */
    class Parabola {
        public:
            Parabola(Point focus);
            Parabola(double x, double y);
            Quadratic to_quadratic(double directrix) const;
            std::vector<Point> get_intersections(const Parabola& p, double directrix) const;
        private:
            Point focus;
    };

#endif
