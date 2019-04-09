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

    Arc* base = NULL;

    FortuneOutput fortuneOutput;
    while(!queue.empty()) {
        Event e = queue.top();
        queue.pop();

        if( e.which() == 0) {
        } else {
            Point pnt = boost::get<Point>(e);
            Point *p = new Point(pnt.get_x(), pnt.get_y());

            if(base == NULL) {
                Parabola *par = new Parabola(*p);
                base = new Arc(*par);
                continue;
            }

            Arc *left = base;
            while(left->left != NULL) {
                left = left->left;
            }

            std::cerr << "UPDATE[NEW POINT]: " << pnt.get_y() << std::endl;
            while(left != NULL) {
                left->update(pnt.get_y());
                left = left->right;
            }
            std::cerr << "DONE" << std::endl;

            Arc* parent = base;
            while(!(parent->underneath(*p))) {
                if(parent->on_left(*p)) {
                    if(!(parent->left)) {
                        parent = parent->left;
                    } else {
                        break;
                    }
                } else {
                    if(!(parent->right)) {
                        parent = parent->right;
                    } else {
                        break;
                    }
                }
            }

            Arc *a, *b, *c;
            a = new Arc(parent->parabola, parent->left, NULL);
            c = new Arc(parent->parabola, NULL, parent->right);
            Parabola *P = new Parabola(*p);
            b = new Arc(*P, a, c);

            if(parent == base) {
                base = b;
            }
        }
    }

	//fortuneOutput.unsolved.push_back(*base);
    Arc *left = base;

    while(left->left != NULL) {
        left = left->left;
    }

    while(left != NULL) {
        //std::cerr << "  unsolved: "  << left->parabola.focus << std::endl;
        fortuneOutput.unsolved.push_back(*left);
        left = left->right;
    }

    return fortuneOutput;
}
