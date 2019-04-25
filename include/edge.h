#ifndef __voronoi_edge_h__
#define __voronoi_edge_h__
    #include <exception>
    #include <optional>
    #include <variant>
    #include "arc.h"
    #include "edge.h"
    #include "vector2d.h"

    class Arc;
    class Edge;

    typedef std::variant<Arc*, Edge*> BeachlineElement;

    class Edge {
        public:
            Edge();
            Edge(const Vector2D&, const Vector2D&);

            Vector2D start, direction;
            std::optional<Vector2D> stop;

            bool operator==(const BeachlineElement&);
            bool operator==(const Edge&);
            Vector2D get_intersection(const Edge& e) const;

            friend std::ostream& operator<<(std::ostream&, const Edge&);
            friend std::string to_string(const Edge&);
            friend bool operator==(const BeachlineElement&, Edge*);
    };
#endif
