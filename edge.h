#ifndef EDGE_H
#define EDGE_H
#include "node.h"
#include "QPair"

class edge {
private:
    QPair<node, node> nodepair;
    double weight;//关系度大小
    QList<String> *value;//用于存储当前边所蕴含关键词
    void setedge(node start_, node end_, double weight_, QList<QString> *value_);

public:
    edge();
    edge(node start_, node end_, double weight_, QList<QString> *value_);
    QList<QString> *getvalue();
    double getweight();
    QPair<node, node >getpair();
};
#endif
