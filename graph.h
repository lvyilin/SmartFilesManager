#ifndef GRAPH_H
#define GRAPH_H
#include "edge.h"
#include "node.h"
#include "QList"
#include "qmath.h"
class graph {
public:
    QList<node> nodelist;
    QList<edge> edgelist;
    graph();
    ~graph();
    void setnodecoordinate(QList<edge> edgelist_);
    void setnode();
    void setedge();
    graph(QList<node> nodelist_, QList<edge> edgrlist_);

};

#endif // GRAPH_H
