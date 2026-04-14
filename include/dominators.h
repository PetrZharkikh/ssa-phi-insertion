#ifndef DOMINATORS_H
#define DOMINATORS_H

#include "cfg.h"

typedef std::unordered_map<Node, std::unordered_set<Node> > DomMap;

// Вычисляет DOM(n) итеративным алгоритмом.
DomMap ComputeDominators(const CFG& cfg, const Node& start);

#endif  // DOMINATORS_H
