#ifndef DOMINATORS_H
#define DOMINATORS_H

#include "cfg.h"

typedef std::unordered_map<Node, std::unordered_set<Node> > DomMap;
typedef std::unordered_map<Node, Node> IdomMap;
typedef std::unordered_map<Node, std::vector<Node> > DomTree;

//вычисляет dom(n) итеративным алгоритмом
DomMap ComputeDominators(const CFG& cfg, const Node& start);

//вычисляет idom(n) на основе множеств dom(n)
IdomMap ComputeImmediateDominators(const CFG& cfg, const DomMap& dom, const Node& start);

//строит дерево доминаторов parent->children
DomTree BuildDominatorTree(const CFG& cfg, const IdomMap& idom, const Node& start);

//вычисляет уровни узлов в дереве доминаторов
std::unordered_map<Node, int> ComputeDomLevels(const CFG& cfg, const DomTree& tree, const Node& start);

#endif  // DOMINATORS_H
