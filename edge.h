#ifndef EDGE_H
#define EDGE_H
#include "node.h"
#include "QPair"

class edge_ {
public:
    node_ *first, *second;
    double weight;//关系度大小
    edge_();
    double getweight();
};
#endif
