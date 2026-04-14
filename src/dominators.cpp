#include "dominators.h"

#include <algorithm>

DomMap ComputeDominators(const CFG& cfg, const Node& start) {
    DomMap dom;

    //инициализация dom
    //dom(start) = {start}, для остальных dom(n) = все узлы
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

            //dom(n) = {n} u пересечение dom(p) для всех p из pred(n)
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

IdomMap ComputeImmediateDominators(const CFG& cfg, const DomMap& dom, const Node& start) {
    IdomMap idom;

    //для start непосредственный доминатор не определяется
    idom[start] = "";

    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        const Node& n = cfg.node_list[i];
        if (n == start) {
            continue;
        }

        //sdom(n) = dom(n) \ {n}
        std::vector<Node> sdom_list;
        for (std::unordered_set<Node>::const_iterator it = dom.at(n).begin(); it != dom.at(n).end(); ++it) {
            if (*it != n) {
                sdom_list.push_back(*it);
            }
        }

        std::sort(sdom_list.begin(), sdom_list.end());

        Node selected = "";
        for (std::size_t c = 0; c < sdom_list.size(); ++c) {
            const Node& candidate = sdom_list[c];
            bool strictly_dominates_someone = false;

            for (std::size_t j = 0; j < sdom_list.size(); ++j) {
                const Node& other = sdom_list[j];
                if (other == candidate) {
                    continue;
                }

                //candidate строго доминирует other, если candidate есть в dom(other)
                if (dom.at(other).find(candidate) != dom.at(other).end()) {
                    strictly_dominates_someone = true;
                    break;
                }
            }

            //idom(n) это элемент sdom(n), который строго не доминирует остальные
            if (!strictly_dominates_someone) {
                selected = candidate;
                break;
            }
        }

        idom[n] = selected;
    }

    return idom;
}

DomTree BuildDominatorTree(const CFG& cfg, const IdomMap& idom, const Node& start) {
    DomTree tree;

    //инициализация всех узлов в дереве
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        tree[cfg.node_list[i]] = std::vector<Node>();
    }

    //добавление ребер parent(idom) -> child
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        const Node& n = cfg.node_list[i];
        if (n == start) {
            continue;
        }

        IdomMap::const_iterator it = idom.find(n);
        if (it != idom.end() && !it->second.empty()) {
            tree[it->second].push_back(n);
        }
    }

    //сортировка детей для стабильного вывода
    for (DomTree::iterator it = tree.begin(); it != tree.end(); ++it) {
        std::sort(it->second.begin(), it->second.end());
    }

    return tree;
}

std::unordered_map<Node, int> ComputeDomLevels(const CFG& cfg, const DomTree& tree, const Node& start) {
    std::unordered_map<Node, int> level;

    //инициализация уровней
    for (std::size_t i = 0; i < cfg.node_list.size(); ++i) {
        level[cfg.node_list[i]] = -1;
    }
    level[start] = 0;

    //обход дерева доминаторов в ширину
    std::vector<Node> q;
    q.push_back(start);

    std::size_t head = 0;
    while (head < q.size()) {
        const Node parent = q[head];
        ++head;

        DomTree::const_iterator it = tree.find(parent);
        if (it == tree.end()) {
            continue;
        }

        for (std::size_t i = 0; i < it->second.size(); ++i) {
            const Node& child = it->second[i];
            level[child] = level[parent] + 1;
            q.push_back(child);
        }
    }

    return level;
}
