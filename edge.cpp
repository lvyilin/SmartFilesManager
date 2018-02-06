#include "edge.h"

edge::edge() {}

edge::edge(node start_, node end_, double weight_, QList<QString> *value_)
{
    setEdge(start_, end_, weight_, *value_);
}

void edge::setedge(node start_, node end_, double weight_, QList<QString> *value_)
{
    if (start_.getid() > end_.getid())
    {
        node Tmp;
        Tmp = start_, start_ = end_, end_ = Tmp;
    }
    nodepair.first = start_, nodepair.second = end_;
    weight = weight_;
    value = value_;

}

QList<QString> *edge::getvalue()
{
    return value;
}

double edge::getweight()
{
    return weight;
}

QPair<node, node>edge::getQPair()
{
    return nodepair;
}
