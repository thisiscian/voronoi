#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <iostream>
#include <list>
#include "parabola.h"

using namespace boost::python;

object pypoint = (class_<Point>("Point", init<double, double>())
    .def_readwrite("x", &Point::x)
    .def_readwrite("y", &Point::y)
    .def("__str__", &Point::as_string)
);

object pyline = (class_<Line>("Line", init<double, double>())
    .def_readwrite("a", &Line::a)
    .def_readwrite("b", &Line::b)
    .def("__str__", &Line::as_string)
    .def("get_y", &Line::get_y)
    .def("get_x", &Line::get_x)
);

typedef std::vector<double> DoubleVector;
object pydvector = (class_<DoubleVector>("DoubleVector")
  .def(vector_indexing_suite<DoubleVector>())
);

typedef std::vector<Point> PointVector;
object pypointvector = (class_<PointVector>("PointVector")
  .def(vector_indexing_suite<PointVector>())
);

template<class T>
list toPythonList(std::vector<T> vector) {
    typename std::vector<T>::iterator iter;
    list pylist;
    for(iter = vector.begin(); iter != vector.end(); ++iter) {
        pylist.append(*iter);
    }
    return pylist;
}

object pyquadratic = (class_<Quadratic>("Quadratic", init<double, double, double>())
    .def_readwrite("a", &Quadratic::a)
    .def_readwrite("b", &Quadratic::b)
    .def_readwrite("c", &Quadratic::c)
    .def("__str__", &Quadratic::as_string)
    .def("get_y", &Quadratic::get_y)
    .def("get_x", &Quadratic::get_x)
);

object pyparabola = (class_<Parabola>("Parabola", init<Point>())
    .def_readwrite("focus", &Parabola::focus)
    .def("to_quadratic", &Parabola::to_quadratic)
    .def("get_intersections", &Parabola::get_intersections)
);


void py_voronoi(const list& points_list) {
    std::list<object> l = std::list<object>();
    stl_input_iterator<object> begin(points_list), end;
    l.assign(begin, end);

    for(const object& o : l) {
        extract<Point> x(o);
        if(x.check()) {
            Point point = x;
            std::cout << "point: " << point << std::endl;
        } else {
            std::cout << "bad point: #" << len(points_list) << std::endl;
        }
    }
}

BOOST_PYTHON_MODULE(pyvoronoi) {
    using namespace boost::python;
    def("solve", py_voronoi);
    def("Point", pypoint);
    def("Line", pyline);
    def("Quadratic", pyquadratic);
    def("Parabola", pyparabola);
}
