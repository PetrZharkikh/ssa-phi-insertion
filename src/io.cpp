#include "io.h"

bool ReadInput(std::istream& in, std::vector<Edge>& edges, std::vector<Node>& defs) {
    int m = 0;
    if (!(in >> m) || m < 0) {
        return false;
    }

    edges.clear();
    edges.reserve(static_cast<std::size_t>(m));

    for (int i = 0; i < m; ++i) {
        Node u;
        Node v;
        if (!(in >> u >> v)) {
            return false;
        }
        edges.push_back(Edge(u, v));
    }

    int k = 0;
    if (!(in >> k) || k < 0) {
        return false;
    }

    defs.clear();
    defs.reserve(static_cast<std::size_t>(k));

    for (int i = 0; i < k; ++i) {
        Node d;
        if (!(in >> d)) {
            return false;
        }
        defs.push_back(d);
    }

    return true;
}
