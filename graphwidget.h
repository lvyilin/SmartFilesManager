#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include "graph.h"
#include"QPainter"
#include "qdebug.h"
#include "dbhelper.h"
#include "QScrollbar"
#include<QMouseEvent>
#include<QWheelEvent>
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
    void graph_change(int ch);
    graph_ *a;

protected:
    void mousePressEvent(QMouseEvent *event); //按下
    void mouseMoveEvent(QMouseEvent *event); //按下移动
    void mouseReleaseEvent(QMouseEvent *event); //松开
    void wheelEvent(QWheelEvent *event);

private:
    DBHelper *dbHelper;
    bool is_drawed = false;
    QPoint offset;//储存鼠标指针位置与窗口位置的差值
    QCursor cursor;//创建光标，保存默认光标形状
signals:

public slots:
};

#endif // GRAPHWIDGET_H
