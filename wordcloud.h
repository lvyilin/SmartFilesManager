#ifndef WORDCLOUD_H
#define WORDCLOUD_H
#include "QString"
#include "QPainter"
#include "QMap"
#include "QTextItem"
#include "QPaintEvent"
#include "utils.h"

class wordcloud {
public:
    wordcloud();
    QString filename;
    FileResult fl;
    QMap<QString/*keyword*/, double/*size*/ > Keywords;
    QList<Label> labels;
    void setwordcloud();
};

#endif // WORDCLOUD_H
