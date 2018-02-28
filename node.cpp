#include "node.h"

node_::node_()
{

}

node_::node_(int id_, QString name_, QList<Relation> relations_, QString path_)
{
    setnode(id_, name_, relations_, path_);
}

int node_::getid()
{
    return id;
}



QString node_::getname()
{
    return name;
}

void node_::setcoordinate(double x_, double y_)
{
    x = x_;
    y = y_;
}

void node_::setnode(int id_, QString name_, QList<Relation> relations_, QString path_)
{
    id = id_;
    name = name_;
    relations = relations_;
    path = path_;
}

bool node_::getis_set()
{
    return is_seted;
}

void node_::changeis_set()
{
    is_seted = true;
}
