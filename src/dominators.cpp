#include "dominators.h"

DomMap ComputeDominators(const CFG& cfg, const Node& start) {
    DomMap dom;

    // Инициализация:
    // DOM(start) = {start}, для остальных DOM(n) = все узлы.
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        const Node& n = cfg.node_list[i];
        if (n == start) {
            dom[n].insert(start);
        } else {
            for (std::size_t j = 0; j < cfg.node_list.size(); ++j) {
                dom[n].insert(cfg.node_list[j]);
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
            const Node& n = cfg.node_list[i];
            if (n == start) {
                continue;
            }

            std::unordered_set<Node> new_dom;
            bool first_pred = true;

            // DOM(n) = {n} U (пересечение DOM(p) для всех p из pred(n)).
            std::unordered_map<Node, std::vector<Node> >::const_iterator pit = cfg.pred.find(n);
            if (pit != cfg.pred.end()) {
                const std::vector<Node>& preds = pit->second;
                for (std::size_t k = 0; k < preds.size(); ++k) {
                    const Node& p = preds[k];
                    if (first_pred) {
                        new_dom = dom[p];
                        first_pred = false;
                    } else {
                        std::unordered_set<Node> intersection;
                        for (std::unordered_set<Node>::const_iterator it = new_dom.begin(); it != new_dom.end(); ++it) {
                            if (dom[p].find(*it) != dom[p].end()) {
                                intersection.insert(*it);
                            }
                        }
                        new_dom = intersection;
                    }
                }
            }

            new_dom.insert(n);
            if (new_dom != dom[n]) {
                dom[n] = new_dom;
                changed = true;
            }
        }
    }

    return dom;
}
