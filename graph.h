#ifndef GRAPH_H
#define GRAPH_H
#include "edge.h"
#include "node.h"
#include "QList"
#include "qmath.h"
#include "utils.h"
#include "dbhelper.h"
#include "qdebug.h"
#include <ogdf/basic/Graph.h>

class graph_ {
public:

    QList<FileResult> frs;
    QList<node_> nodelist;
    QList<edge_> edgelist;
    graph_(DBHelper *db);
    ~graph_();
    void setnode();
    void setedge();
    void start();
    void setnodecoordinate();
private:
    DBHelper *dbHelper;


};

#endif // GRAPH_H
