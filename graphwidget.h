#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "graph.h"
#include"QPainter"
#include "qdebug.h"
#include "dbhelper.h"
#include "QScrollbar"

class graphwidget : public QWidget {
    Q_OBJECT
public:
    explicit graphwidget(QWidget *parent = nullptr, DBHelper *db = nullptr);
    ~graphwidget();
    void setgraph();
    void paintEvent(QPaintEvent *event);
    graph *a;

private:
    DBHelper *dbHelper;
signals:

public slots:
};

#endif // GRAPHWIDGET_H
