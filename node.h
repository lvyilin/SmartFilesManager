#ifndef NODE_H
#define NODE_H
#include "QString"
#include "QList"
#include "utils.h"
#include "ogdf/basic/Graph.h"


class node_ {
public:
    int id;
    QString name;
    QString path;
    ogdf::node ogdfID;
    double x, y; //用于画图
    void setnode(int id_, QString name_, QList<Relation> relations_, QString path_);
    bool is_seted;
    QList<node_> connectednode;
    QList<Label> labels;
    QList<Relation> relations;

    node_();
    node_(int id_, QString name_, QList<Relation> relations_, QString path_);

    int getid();
    QString getname();
    void setcoordinate(double x_, double y_);
    bool getis_set();
    void changeis_set();

};

#endif // NODE_H
