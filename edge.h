#ifndef EDGE_H
#define EDGE_H
#include "node.h"
#include "QPair"

class edge {
public:
    QPair<node, node> nodepair;
    double weight;//关系度大小
    void setedge(node start_, node end_, double weight_);
    edge();
    edge(node start_, node end_, double weight_);
    double getweight();
    QPair<node, node >getpair();
};
#endif
