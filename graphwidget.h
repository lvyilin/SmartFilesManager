#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "graph.h"
#include"QPainter"

class graphwidget : public QWidget {
    Q_OBJECT
public:
    explicit graphwidget(QWidget *parent = nullptr);
    ~graphwidget();
    void setgraph();
    void paintEvent(QPaintEvent *event);
    void setnodecoordinate();
    graph *a;
private:

signals:

public slots:
};

#endif // GRAPHWIDGET_H
