#ifndef IO_H
#define IO_H

#include <istream>
#include <vector>

#include "cfg.h"

//читает входные данные из потока
//формат:
//m
//u1 v1
//...
//um vm
//k
//d1 d2 ... dk
bool ReadInput(std::istream& in, std::vector<Edge>& edges, std::vector<Node>& defs);

#endif  // IO_H
