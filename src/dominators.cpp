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

std::vector<Edge> ComputeJEdges(const CFG& cfg, const DomMap& dom) {
    std::vector<Edge> j_edges;

    //перебор всех дуг исходного cfg
    for (std::unordered_map<Node, std::vector<Node> >::const_iterator it = cfg.succ.begin(); it != cfg.succ.end(); ++it) {
        const Node& x = it->first;
        const std::vector<Node>& out = it->second;

        for (std::size_t i = 0; i < out.size(); ++i) {
            const Node& y = out[i];

            //проверка x !sdom y
            bool x_strictly_dominates_y = false;
            if (x != y && dom.at(y).find(x) != dom.at(y).end()) {
                x_strictly_dominates_y = true;
            }

            if (!x_strictly_dominates_y) {
                j_edges.push_back(Edge(x, y));
            }
        }
    }

    //сортировка нужна для стабильного вывода
    std::sort(j_edges.begin(), j_edges.end());
    return j_edges;
}

static void CollectSubtreeNodes(
    const Node& root,
    const DomTree& tree,
    std::unordered_set<Node>& subtree) {
    if (subtree.find(root) != subtree.end()) {
        return;
    }

    subtree.insert(root);
    DomTree::const_iterator it = tree.find(root);
    if (it == tree.end()) {
        return;
    }

    for (std::size_t i = 0; i < it->second.size(); ++i) {
        CollectSubtreeNodes(it->second[i], tree, subtree);
    }
}

std::vector<Node> ComputeDominanceFrontierForNode(
    const Node& x,
    const DomTree& tree,
    const std::unordered_map<Node, int>& level,
    const std::vector<Edge>& j_edges) {
    std::unordered_set<Node> subtree;
    std::unordered_set<Node> df_set;

    //сбор узлов subtree(x)
    CollectSubtreeNodes(x, tree, subtree);

    //отбор y по j-edge z->y, где z в subtree(x) и level(y) <= level(x)
    for (std::size_t i = 0; i < j_edges.size(); ++i) {
        const Node& z = j_edges[i].first;
        const Node& y = j_edges[i].second;

        if (subtree.find(z) == subtree.end()) {
            continue;
        }

        std::unordered_map<Node, int>::const_iterator y_level_it = level.find(y);
        std::unordered_map<Node, int>::const_iterator x_level_it = level.find(x);
        if (y_level_it == level.end() || x_level_it == level.end()) {
            continue;
        }

        if (y_level_it->second <= x_level_it->second) {
            df_set.insert(y);
        }
    }

    //преобразование в отсортированный вектор
    std::vector<Node> result(df_set.begin(), df_set.end());
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<Node> ComputeIteratedDominanceFrontier(
    const std::vector<Node>& defs,
    const DomTree& tree,
    const std::unordered_map<Node, int>& level,
    const std::vector<Edge>& j_edges) {
    std::unordered_set<Node> defs_set(defs.begin(), defs.end());
    std::unordered_set<Node> idf_set;
    std::unordered_set<Node> in_worklist;
    std::vector<Node> worklist;

    //инициализация рабочего списка defs
    for (std::size_t i = 0; i < defs.size(); ++i) {
        if (in_worklist.insert(defs[i]).second) {
            worklist.push_back(defs[i]);
        }
    }

    //итеративное вычисление idf
    std::size_t head = 0;
    while (head < worklist.size()) {
        const Node x = worklist[head];
        ++head;

        std::vector<Node> df = ComputeDominanceFrontierForNode(x, tree, level, j_edges);
        for (std::size_t i = 0; i < df.size(); ++i) {
            const Node& y = df[i];

            if (idf_set.insert(y).second) {
                if (defs_set.find(y) == defs_set.end()) {
                    if (in_worklist.insert(y).second) {
                        worklist.push_back(y);
                    }
                }
            }
        }
    }

    //сортировка результата для стабильного вывода
    std::vector<Node> result(idf_set.begin(), idf_set.end());
    std::sort(result.begin(), result.end());
    return result;
}
