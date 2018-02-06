#ifndef NODE_H
#define NODE_H
#include "QString"
#include "QList"

class node {
private:
    int id;
    QString name;
    QPair<double, double > coordinate; //用于画图
    void setnode(int id_, QString name_, QList *value_);

public:
    bool is_seted;
    QList<node> connectednode;

    node();
    node(int id_, QString name_, QList *value);

    int getid();
    QString getname();
    QPair<double, double> getcoordinate();
    void setcoordinate();

};

#endif // NODE_H
