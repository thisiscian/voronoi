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

Arc::Arc(Parabola &p): parabola(p), left(NULL), right(NULL) {
}

Arc::Arc(Parabola &p, Arc* l, Arc* r): parabola(p), left(l), right(r) {
}

FortuneOutput::FortuneOutput() {
	unsolved = std::vector<Point>();
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

		if(left_p.y > right_p.y) {
			return true;
		} else if(left_p.y == right_p.y && left_p.x > right_p.x) {
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
	Arc base = Arc(p);
	queue.pop();

	FortuneOutput fortuneOutput;
	while(!queue.empty()) {
		Event e = queue.top();
		queue.pop();

		if( e.which() == 0) {
		} else {
			Point p = boost::get<Point>(e);
			fortuneOutput.unsolved.push_back(p);
		}
	}
	return fortuneOutput;
}
