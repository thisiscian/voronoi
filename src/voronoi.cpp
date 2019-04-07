#include "voronoi.h"

Edge::Edge() {
    start = Point();
    direction = Point();
}

Edge::Edge(Point new_start, Point new_direction) {
    start = new_start;
    direction = new_direction;
}

const std::string Edge::as_string() const {
    std::stringstream ss;
    ss << "Edge(" << start << " > " << (stop.is_initialized() ? stop.get() : direction) << ")";
    return ss.str();
}

bool Edge::operator==(const Edge& e) const {
    return e.start == start && e.direction == direction && e.stop == e.stop;
}

FortuneOutput::FortuneOutput() {
    unsolved = std::vector<Arc>();
    edges = std::vector<Edge>();
}

typedef boost::variant<Parabola, Point> Event;
struct EventPointVisitor : public boost::static_visitor<Point> {
    Point operator() (const Point& p) { return p; }
    Point operator() (const Parabola& p) { return p.focus; }
};

FortuneOutput fortune_solve(std::vector<Point> points) {
	auto point_compare = [](Event& left, Event& right){
        EventPointVisitor epv;
        Point left_p = left.apply_visitor(epv);
        Point right_p = right.apply_visitor(epv);

        if(left_p.get_y() < right_p.get_y()) {
            return true;
        } else if(left_p.get_y() == right_p.get_y() && left_p.get_x() < right_p.get_x()) {
            return true;
        } else {
            return false;
        }
    };

    std::priority_queue<Event, std::vector<Event>, decltype(point_compare)> queue(point_compare);

    for(const Point& p: points) {
        queue.push(Event(p));
    }

    Parabola p = Parabola(boost::get<Point>(queue.top()));
    Arc* base = new Arc(p);
    std::cout << (*base) << std::endl;

    queue.pop();

    FortuneOutput fortuneOutput;
    while(!queue.empty()) {
        Event e = queue.top();
        queue.pop();

        if( e.which() == 0) {
        } else {
            Point p = boost::get<Point>(e);
            Arc* parent = base;
            while(!parent->underneath(p)) {
                if(parent->on_left(p)) {
                    parent = parent->left;
                } else {
                    parent = parent->right;
                }
            }

            Arc a(parent->parabola, parent->left, NULL);
            Arc c(parent->parabola, NULL, parent->right);
            Parabola P(p);
            Arc b(P, &a, &c);

            a.update(p.get_y());
            c.update(p.get_y());

            if(parent==base) {
                base = &b;
            }
        }
    }

    fortuneOutput.unsolved.push_back(*base);
    Arc *left = base->left;
    while(left != NULL) {
        fortuneOutput.unsolved.push_back(*left);
        left = left->left;
    }

    Arc *right = base->right;
    while(right != NULL) {
        fortuneOutput.unsolved.push_back(*right);
        right = right->right;
    }

    return fortuneOutput;
}
