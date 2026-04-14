#include "cfg.h"

#include <algorithm>

CFG BuildCFG(const std::vector<Edge>& edges) {
    CFG cfg;

    //построение succ/pred и множества узлов
    for (std::size_t i = 0; i < edges.size(); ++i) {
        const Node& u = edges[i].first;
        const Node& v = edges[i].second;
        cfg.succ[u].push_back(v);
        cfg.pred[v].push_back(u);
        cfg.nodes.insert(u);
        cfg.nodes.insert(v);
    }

    //упорядоченный список нужен для стабильного вывода
    cfg.node_list.assign(cfg.nodes.begin(), cfg.nodes.end());
    std::sort(cfg.node_list.begin(), cfg.node_list.end());

    return cfg;
}
