#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#ifndef EDGE_H
#define EDGE_H
#include "node.h"
#include "QPair"

class edge_ {
public:
    node_ *first, *second;
    double weight;//关系度大小
    edge_();
    double getweight();
};
#endif
