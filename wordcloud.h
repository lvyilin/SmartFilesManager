#ifndef WORDCLOUD_H
#define WORDCLOUD_H
#include "QString"
#include "QPainter"
#include "QMap"
#include "QTextItem"
#include "QPaintEvent"

class wordcloud {
public:
    wordcloud();
    QString filename;
    QMap<QString/*keyword*/, double/*size*/ > Keywords;
    void setwordcloud(QString filename_, QMap<QString/*keyword*/, double/*size*/ > Keywords_);
};

#endif // WORDCLOUD_H
