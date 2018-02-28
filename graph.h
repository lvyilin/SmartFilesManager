#ifndef GRAPH_H
#define GRAPH_H
#include "edge.h"
#include "node.h"
#include "QList"
#include "qmath.h"
#include "utils.h"
#include "dbhelper.h"
#include "qdebug.h"

class graph {
public:

    QList<FileResult> frs;
    QList<node> nodelist;
    QList<edge> edgelist;
    graph(DBHelper *db);
    ~graph();
    void setnode();
    void setedge();
    void start();
    void setnodecoordinate();
private:
    DBHelper *dbHelper;


};

#endif // GRAPH_H
