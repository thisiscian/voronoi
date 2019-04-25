#ifndef __voronoi_vector2d_h__
#define __voronoi_vector2d_h__
    #include <float.h>
    #include <exception>
    #include <cmath>
    #include <cstdlib>
    #include <ostream>
    #include <sstream>
    #include <string>

    class Vector2D {
        public:
            Vector2D();
            Vector2D(double, double);
            Vector2D(const Vector2D&);

            double x, y;

            Vector2D get_normal() const;
            Vector2D get_unit_vector() const;
            double get_distance(const Vector2D&) const;
            double get_length() const;
            double get_sq_length() const;
            bool operator==(const Vector2D&) const;
            Vector2D operator+(const Vector2D&) const;
            Vector2D operator-(const Vector2D&) const;
            bool operator<(const Vector2D&) const;

            friend Vector2D operator*(double, const Vector2D&);
            friend std::ostream& operator<<(std::ostream&, const Vector2D&);
            friend std::string to_string(const Vector2D&);

            static Vector2D get_random(double, double);
    };


    Vector2D get_circumcenter(const Vector2D&, const Vector2D&, const Vector2D&);
#endif
