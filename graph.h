#ifndef GRAPH_H
#define GRAPH_H
#include "edge.h"
#include "node.h"
#include "QList"
#include "QPainter"
#define size 30;
class graph {
public:
    QList<node> nodelist;
    QList<edge> edgelist;
    graph();
    graph(QList<node> nodelist_, QList<edge> edgrlist_);
    void drawgraph(QPaintEvent *event);

};

#endif // GRAPH_H
