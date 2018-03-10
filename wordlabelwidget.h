#ifndef WORDLABELWIDGET_H
#define WORDLABELWIDGET_H

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
#include "confighelper.h"
#include "utils.h"
#include <QList>
#include <node.h>

class wordlabelwidget : public QWidget {
    Q_OBJECT
public:
    explicit wordlabelwidget(QWidget *parent, DBHelper *db, ConfigHelper *cf);
    ~wordlabelwidget();
    void setpath(QString filepath_);
    void paintEvent(QPaintEvent *event);
    void setText(QString name, QString info);
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
    QList<FileResult> frs;
    bool is_drawed = false;
    QPoint offset;//储存鼠标指针位置与窗口位置的差值
    QCursor cursor;//创建光标，保存默认光标形状
    QRect rec;
    QPoint h_Point;
    QLabel *labelName;
    QLabel *labelInfo;
    QHBoxLayout *horLayout;
    QVBoxLayout *verlayout;
    QString filename ;
    QList<Label> labellist;
    int id;
    QList<File> l_file;
    QStringList files;
    QString filepath;

public slots:
};

#endif // WORDLABELWIDGET_H
