#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "graph.h"
#include"QPainter"
#include "qdebug.h"
#include "dbhelper.h"
#include "QScrollbar"
#define NORMALIZED_GRAPH_WIDTH_AND_HEIGHT 500
#define NORMALIZED_GRAPH_WIDTH_AND_HEIGHT_HALF 250
#define NORMALIZED_GRAPH_LEFT 30
#define NORMALIZED_GRAPH_TOP 60

class graphwidget : public QWidget {
    Q_OBJECT
public:
    explicit graphwidget(QWidget *parent = nullptr, DBHelper *db = nullptr);
    ~graphwidget();
    void setgraph();
    void paintEvent(QPaintEvent *event);
    void dolayout();
    graph_ *a;

private:
    DBHelper *dbHelper;
signals:

public slots:
};

#endif // GRAPHWIDGET_H
