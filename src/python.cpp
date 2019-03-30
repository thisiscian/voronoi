#include <boost/python.hpp>
#include <iostream>
#include <list>
#include "parabola.h"

using namespace boost::python;

object pypoint = (class_<Point>("Point", init<double, double>())
    .def_readonly("x", &Point::x)
    .def_readonly("y", &Point::y)
);

void py_voronoi(const list& points_list) {
    std::list<object> l = std::list<object>();
    stl_input_iterator<object> begin(points_list), end;
    l.assign(begin, end);

    for(const object& o : l) {
        extract<double> x(o);
        if(x.check()) {
            double point = x;
            std::cout << "point: " << std::endl;
        } else {
            std::cout << "bad point: #" << len(points_list) << std::endl;
        }
    }
}

char const* greet() {
    return "hello, world";
}

BOOST_PYTHON_MODULE(pyvoronoi) {
    using namespace boost::python;
    def("greet", greet);
    def("solve", py_voronoi);
    def("point", pypoint);
}
