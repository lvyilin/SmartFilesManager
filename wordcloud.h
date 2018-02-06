#ifndef WORDCLOUD_H
#define WORDCLOUD_H
#include "QString"
#include "QPainter"
#include "QMap"
#include "QTextItem"

class wordcloud {
public:
    wordcloud();
    QString filename;
    QMap<QString/*keyword*/, double/*size*/ > Keywords;
    void setwordcloud(QString filename_, QMap<QString/*keyword*/, double/*size*/ > Keywords_);
    void drawwordcloud(QPaintEvent *event);
};

#endif // WORDCLOUD_H
