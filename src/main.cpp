#include <iostream>
#include <set>
#include <vector>

#include "cfg.h"
#include "dominators.h"

int main() {
    // Шаг 1. Описание дуг CFG.
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

    // Шаг 2. Построение структур CFG.
    CFG cfg = BuildCFG(edges);

    std::cout << "Nodes:\n";
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        std::cout << "  " << cfg.node_list[i] << '\n';
    }

    std::cout << "\nSucc:\n";
    for (std::unordered_map<Node, std::vector<Node> >::const_iterator it = cfg.succ.begin(); it != cfg.succ.end(); ++it) {
        const Node& u = it->first;
        const std::vector<Node>& vs = it->second;
        std::cout << "  " << u << " -> ";
        for (std::size_t i = 0; i < vs.size(); ++i) {
            std::cout << vs[i] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nPred:\n";
    for (std::unordered_map<Node, std::vector<Node> >::const_iterator it = cfg.pred.begin(); it != cfg.pred.end(); ++it) {
        const Node& v = it->first;
        const std::vector<Node>& us = it->second;
        std::cout << "  " << v << " <- ";
        for (std::size_t i = 0; i < us.size(); ++i) {
            std::cout << us[i] << ' ';
        }
        std::cout << '\n';
    }

    // Шаг 3. Итеративное вычисление DOM(n).
    DomMap dom = ComputeDominators(cfg, "start");

    std::cout << "\nDOM sets:\n";
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        const Node& n = cfg.node_list[i];
        std::set<Node> ordered_dom(dom[n].begin(), dom[n].end());
        std::cout << "  DOM(" << n << ") = {";

        bool first = true;
        for (std::set<Node>::const_iterator it = ordered_dom.begin(); it != ordered_dom.end(); ++it) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << *it;
            first = false;
        }
        std::cout << "}\n";
    }

    return 0;
}
