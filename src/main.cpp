#include <iostream>
#include <set>
#include <vector>

#include "cfg.h"
#include "dominators.h"

int main() {
    //описание дуг CFG
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

    //построение структур cfg
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

    //итеративное вычисление dom(n)
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

    //построение idom и дерева доминаторов
    IdomMap idom = ComputeImmediateDominators(cfg, dom, "start");
    DomTree dom_tree = BuildDominatorTree(cfg, idom, "start");
    std::unordered_map<Node, int> level = ComputeDomLevels(cfg, dom_tree, "start");
    std::vector<Edge> j_edges = ComputeJEdges(cfg, dom);

    std::cout << "\nIDOM:\n";
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        const Node& n = cfg.node_list[i];
        std::cout << "  idom(" << n << ") = ";
        if (n == "start") {
            std::cout << "-";
        } else {
            std::cout << idom[n];
        }
        std::cout << '\n';
    }

    std::cout << "\nDominator tree:\n";
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        const Node& p = cfg.node_list[i];
        std::cout << "  " << p << " -> ";
        for (std::size_t j = 0; j < dom_tree[p].size(); ++j) {
            std::cout << dom_tree[p][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\nLevels:\n";
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        const Node& n = cfg.node_list[i];
        std::cout << "  level(" << n << ") = " << level[n] << '\n';
    }

    std::cout << "\nJ-edges:\n";
    for (std::size_t i = 0; i < j_edges.size(); ++i) {
        std::cout << "  {" << j_edges[i].first << ", " << j_edges[i].second << "}\n";
    }

    return 0;
}
