#include "node.h"

node::node(int id_, QString name_, QList *value_)
{
    setnode(id_, QStringname_, QList * value);
}

void node::setnodenode(int id_, QString name_, QList *value_)
{
    id = id_;
    name = name_;
    value = value_;
}

int node::getid()
{
    return id;
}

QPair<double, double> node::getcoordinate()
{
    return coordinate;
}

QString node::getname()
{
    return name;
}

void node::setcoordinate(double x_, double y_)
{
    coordinate.first = x_;
    coordinate.second = y_;
}
