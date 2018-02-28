#ifndef WORDCLOUDWIDGET_H
#define WORDCLOUDWIDGET_H

#include <QWidget>
#include "wordcloud.h"
#include "Qpainter"
#include "utils.h"
#include "qdebug.h"


class wordcloudwidget : public QWidget {
    Q_OBJECT
public:
    explicit wordcloudwidget(QWidget *parent = nullptr);
    ~wordcloudwidget();
    void setwordcloud();
    void paintEvent(QPaintEvent *event);

    wordcloud *a;

private:

signals:

public slots:
};

#endif // WORDCLOUDWIDGET_H
