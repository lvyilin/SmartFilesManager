#ifndef LABELSWIDEGET_H
#define LABELSWIDEGET_H

#include <QWidget>
#include <QPainter>
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

class labelswideget : public QWidget {
    Q_OBJECT
public:
    explicit labelswideget(QWidget *parent = nullptr, DBHelper *db = nullptr, ConfigHelper *cf = nullptr);
    void paintEvent(QPaintEvent *event);
    QString choosecolor(int i);

protected:
    void mousePressEvent(QMouseEvent *event); //按下
    void mouseMoveEvent(QMouseEvent *event); //按下移动
    void mouseReleaseEvent(QMouseEvent *event); //松开
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    DBHelper *dbHelper;
    ConfigHelper *configHelper;
    QHBoxLayout *horLayout;
    QVBoxLayout *verlayout;
    QLabel *labelName;
    QLabel *labelInfo;
    QRect rec;
    QPoint offset;//储存鼠标指针位置与窗口位置的差值
    QCursor cursor;//创建光标，保存默认光标形状
    bool is_drawed = false;
signals:

public slots:
};

#endif // LABELSWIDEGET_H
