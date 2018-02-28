#include "node.h"

node::node()
{

}

node::node(int id_, QString name_, QList<Relation> relations_, QString path_)
{
    setnode(id_, name_, relations_, path_);
}

int node::getid()
{
    return id;
}



QString node::getname()
{
    return name;
}

void node::setcoordinate(double x_, double y_)
{
    x = x_;
    y = y_;
}

void node::setnode(int id_, QString name_, QList<Relation> relations_, QString path_)
{
    id = id_;
    name = name_;
    relations = relations_;
    path = path_;
}

bool node::getis_set()
{
    return is_seted;
}

void node::changeis_set()
{
    is_seted = true;
}
