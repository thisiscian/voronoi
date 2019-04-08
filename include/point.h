#ifndef __voronoi_point__
#define __voronoi_point__

	#include <cmath>
    #include "base.h"

    class Point {
        public:
            Point();
            Point(double X, double Y);
            const std::string as_string() const;

            bool operator==(const Point& p) const;
            friend std::ostream& operator<<(std::ostream&, const Point& p);

            double get_x() const;
            void set_x(double value);

            double get_y() const;
            void set_y(double value);

			double distance_to(const Point& p) const;
			double length() const;

        private:
            double x, y;
    };

#endif
