#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include "point.h"
#include "line.h"
#include "quadratic.h"
#include "parabola.h"
#include "voronoi.h"

using namespace boost::python;

typedef std::vector<double> DoubleVector;
typedef std::vector<Point> PointVector;
typedef std::vector<Arc> ArcVector;
typedef std::vector<Edge> EdgeVector;


template<class T>
list toPythonList(std::vector<T> vector) {
    typename std::vector<T>::iterator iter;
    list pylist;
    for(iter = vector.begin(); iter != vector.end(); ++iter) {
        pylist.append(*iter);
    }
    return pylist;
}

FortuneOutput py_voronoi(const list& points_list) {
    std::list<object> l = std::list<object>();
    stl_input_iterator<object> begin(points_list), end;
    l.assign(begin, end);

    std::vector<Point> points;
    for(const object& o : l) {
        extract<Point> x(o);
        if(x.check()) {
            points.push_back(x);
        }
    }

    FortuneOutput fortune_output = fortune_solve(points);
    return fortune_output;
}

Parabola ARC_get_parabola( Arc const& a) {
    return a.parabola;
}

Arc const& ARC_get_right(Arc const& a) {
    return *(a.right);
}

Arc const& ARC_get_left(Arc const& a) {
    return *(a.left);
}

BOOST_PYTHON_MODULE(pyvoronoi) {
    using namespace boost::python;

    class_<Point>("Point", init<double, double>())
        .def("__str__", &Point::as_string)
        .add_property("x", &Point::get_x, &Point::set_x)
        .add_property("y", &Point::get_y, &Point::set_y);

    class_<DoubleVector>("DoubleVector")
        .def(vector_indexing_suite<DoubleVector>());

    class_<PointVector>("PointVector")
        .def(vector_indexing_suite<PointVector>());

    class_<EdgeVector>("EdgeVector")
        .def(vector_indexing_suite<EdgeVector>());

    class_<FortuneOutput>("FortuneOutput")
        .def_readonly("unsolved", &FortuneOutput::unsolved)
        .def_readonly("edges", &FortuneOutput::edges);

    class_<Arc>("Arc", init<Point>())
        .def("__str__", &Arc::as_string)
        .add_property("parabola", make_function(ARC_get_parabola)) //, return_value_policy<copy_const_reference>()))
        .add_property("left", make_function(ARC_get_left, return_value_policy<copy_const_reference>()))
        .add_property("right", make_function(ARC_get_right, return_value_policy<copy_const_reference>()));

    def("solve", py_voronoi);

    class_<Equation>("Equation")
        .def("__str__", &Equation::as_string)
        .def("__getitem__", &Equation::get_coefficient)
        .def("get_coefficient", &Equation::get_coefficient)
        .def("get_coefficient_count", &Equation::get_coefficient_count)
        .def("coefficient_count", &Equation::get_coefficient_count)
        .def("get_y", &Equation::get_y)
        .def("get_x", &Equation::get_x);

    class_<Line, bases<Equation> >("Line", init<double, double>());
    class_<Quadratic, bases<Equation> >("Quadratic", init<double, double, double>());

    class_<Edge>("Edge", init<Point, Point>())
        .def_readwrite("start", &Edge::start)
        .def_readwrite("direction", &Edge::direction)
      //.def_readwrite("stop", &Edge::y)
        .def("__str__", &Edge::as_string);

    class_<Parabola>("Parabola", init<Point>())
        //.add_property("focus", make_function(&Parabola::focus, return_value_policy<reference_existing_object>()))
        .def("to_quadratic", &Parabola::to_quadratic)
        .def("get_intersections", &Parabola::get_intersections);
}
