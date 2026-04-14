#ifndef CFG_H
#define CFG_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

typedef std::string Node;
typedef std::pair<Node, Node> Edge;

struct CFG {
    std::unordered_map<Node, std::vector<Node> > succ;
    std::unordered_map<Node, std::vector<Node> > pred;
    std::unordered_set<Node> nodes;
    std::vector<Node> node_list;
};

// Формирует структуры CFG из списка дуг.
CFG BuildCFG(const std::vector<Edge>& edges);

#endif  // CFG_H
