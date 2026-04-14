#include <iostream>
#include <vector>

#include "cfg.h"
#include "dominators.h"
#include "io.h"

int main() {
    std::vector<Edge> edges;
    std::vector<Node> defs;

    //чтение входных данных
    if (!ReadInput(std::cin, edges, defs)) {
        std::cerr << "input error\n";
        return 1;
    }

    //базовые структуры cfg
    CFG cfg = BuildCFG(edges);

    //вычисление доминаторов и служебных структур
    DomMap dom = ComputeDominators(cfg, "start");
    IdomMap idom = ComputeImmediateDominators(cfg, dom, "start");
    DomTree dom_tree = BuildDominatorTree(cfg, idom, "start");
    std::unordered_map<Node, int> level = ComputeDomLevels(cfg, dom_tree, "start");
    std::vector<Edge> j_edges = ComputeJEdges(cfg, dom);

    //вычисление узлов для вставки phi
    std::vector<Node> phi_nodes = ComputeIteratedDominanceFrontier(defs, dom_tree, level, j_edges);

    //вывод результата
    std::cout << "{";
    for (std::size_t i = 0; i < phi_nodes.size(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << phi_nodes[i];
    }
    std::cout << "}\n";

    return 0;
}
