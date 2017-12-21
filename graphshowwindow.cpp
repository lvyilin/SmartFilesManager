#include "graphshowwindow.h"
#include "ui_graphshowwindow.h"

GraphShowWindow::GraphShowWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphShowWindow)
{
    ui->setupUi(this);
}

GraphShowWindow::~GraphShowWindow()
{
    delete ui;
}
