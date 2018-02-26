#ifndef NODE_H
#define NODE_H
#include "QString"
#include "QList"

class node {
public:
    int id;
    QString name;
    QPair<double/*x*/, double/*y*/ > coordinate; //用于画图
    void setnode(int id_, QString name_);
    bool is_seted;
    QList<node> connectednode;

    node();
    node(int id_, QString name_);

    int getid();
    QString getname();
    QPair<double, double> getcoordinate();
    void setcoordinate(double x_, double y_);
    bool getis_set();
    void changeis_set();

};

#endif // NODE_H
