#include "node.h"

node::node()
{

}

node::node(int id_, QString name_)
{
    setnode(id_, name_);
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

void node::setnode(int id_, QString name_)
{
    id = id_;
    name = name_;
}

bool node::getis_set()
{
    return is_seted;
}

void node::changeis_set()
{
    is_seted = true;
}
