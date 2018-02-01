#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <QStringList>
#include "deps/cppjieba/Jieba.hpp"
#include "utils.h"

//Singleton class
class Toolkit {

public:
    static Toolkit &getInstance();
    Toolkit(Toolkit const &) = delete;            // Copy construct
    Toolkit(Toolkit &&) = delete;                 // Move construct
    Toolkit &operator=(Toolkit const &) = delete; // Copy assign
    Toolkit &operator=(Toolkit &&) = delete;      // Move assign

    QStringList getSegment(QString text);
    QMap<QString, double> getKeywords(QString text);
private:
    Toolkit();
    cppjieba::Jieba *jieba;

};

#endif // TOOLKIT_H
