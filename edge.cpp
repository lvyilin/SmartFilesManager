#include "edge.h"

edge::edge() {}

edge::edge(node start_, node end_, double weight_)
{
    setedge(start_, end_, weight_);
}

void edge::setedge(node start_, node end_, double weight_)
{
    if (start_.getid() > end_.getid())
    {
        node Tmp;
        Tmp = start_, start_ = end_, end_ = Tmp;
    }
    nodepair.first = start_, nodepair.second = end_;
    weight = weight_;
}

double edge::getweight()
{
    return weight;
}

QPair<node, node>edge::getpair()
{
    return nodepair;
}
