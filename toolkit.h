#ifndef TOOLKIT_H
#define TOOLKIT_H

#include "deps/cppjieba/Jieba.hpp"

class Toolkit {
public:
    Toolkit();
private:
    cppjieba::Jieba *jieba;
};

#endif // TOOLKIT_H
