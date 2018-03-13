#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#ifndef LABELGRAPHWIDGET_H
#define LABELGRAPHWIDGET_H

#include <QWidget>
#include <QListWidget>
#include "graph.h"
#include"QPainter"
#include "qdebug.h"
#include "dbhelper.h"
#include "QScrollbar"
#include "QLabel"
#include<QMouseEvent>
#include<QWheelEvent>
#include "QStatusBar"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "QTimer"
#include "confighelper.h"
#include "QStringList"
#define NORMALIZED_GRAPH_WIDTH_AND_HEIGHT 400
#define NORMALIZED_GRAPH_WIDTH_AND_HEIGHT_HALF 300
#define NORMALIZED_GRAPH_LEFT 30
#define NORMALIZED_GRAPH_TOP 60
#define transformX(x) (((x)+viewCenterX)*viewScale)
#define transformY(y) (((y)+viewCenterY)*viewScale)

class labelgraphwidget : public QWidget {
    Q_OBJECT
public:
    explicit labelgraphwidget(QWidget *parent = nullptr, DBHelper *db = nullptr, ConfigHelper *cf = nullptr);
    ~labelgraphwidget();
    void setgraph();
    void paintEvent(QPaintEvent *event);
    void dolayout();
    void graph_change(int ch);
    void setname(QString thelabelname);
    int adjustViewScale(double k_);
    void setText(QString name, QString info);
    void changepoint(int i);
    QString choosecolor(int i);
    graph_ *a;
    QTimer *timer;
    QListWidget *list;


protected:
    void mousePressEvent(QMouseEvent *event); //按下
    void mouseMoveEvent(QMouseEvent *event); //按下移动
    void mouseReleaseEvent(QMouseEvent *event); //松开
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    DBHelper *dbHelper;
    ConfigHelper *configHelper;
    bool is_drawed = false;
    bool is_showinf = false;
    QPoint offset;//储存鼠标指针位置与窗口位置的差值
    QCursor cursor;//创建光标，保存默认光标形状
    QRect rec;
    QPoint h_Point;
    QLabel *labelName;
    QLabel *labelInfo;
    int r = 5;
    QHBoxLayout *horLayout;
    QVBoxLayout *verlayout;
    int z_temp;
    QString labelname_;
    QStringList inf;

public slots:
};
#endif // LABELGRAPHWIDGET_H
