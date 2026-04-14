#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

typedef std::string Node;
typedef std::pair<Node, Node> Edge;

int main() {
    std::vector<Edge> edges;
    edges.push_back(Edge("start", "1"));
    edges.push_back(Edge("1", "2"));
    edges.push_back(Edge("1", "3"));
    edges.push_back(Edge("2", "4"));
    edges.push_back(Edge("3", "4"));
    edges.push_back(Edge("3", "5"));
    edges.push_back(Edge("4", "1"));
    edges.push_back(Edge("4", "5"));
    edges.push_back(Edge("5", "stop"));

    std::unordered_map<Node, std::vector<Node> > succ;
    std::unordered_map<Node, std::vector<Node> > pred;
    std::unordered_set<Node> nodes;

    for (std::size_t i = 0; i < edges.size(); ++i) {
        const Node& u = edges[i].first;
        const Node& v = edges[i].second;
        succ[u].push_back(v);
        pred[v].push_back(u);
        nodes.insert(u);
        nodes.insert(v);
    }

    std::cout << "Nodes:\n";
    for (std::unordered_set<Node>::const_iterator it = nodes.begin(); it != nodes.end(); ++it) {
        std::cout << "  " << *it << '\n';
    }

    std::cout << "\nSucc:\n";
    for (std::unordered_map<Node, std::vector<Node> >::const_iterator it = succ.begin(); it != succ.end(); ++it) {
        const Node& u = it->first;
        const std::vector<Node>& vs = it->second;
        std::cout << "  " << u << " -> ";
        for (std::size_t i = 0; i < vs.size(); ++i) {
            std::cout << vs[i] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nPred:\n";
    for (std::unordered_map<Node, std::vector<Node> >::const_iterator it = pred.begin(); it != pred.end(); ++it) {
        const Node& v = it->first;
        const std::vector<Node>& us = it->second;
        std::cout << "  " << v << " <- ";
        for (std::size_t i = 0; i < us.size(); ++i) {
            std::cout << us[i] << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
