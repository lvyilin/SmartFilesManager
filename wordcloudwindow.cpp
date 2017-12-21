#include "wordcloudwindow.h"
#include "ui_wordcloudwindow.h"

WordCloudWindow::WordCloudWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WordCloudWindow)
{
    ui->setupUi(this);
}

WordCloudWindow::~WordCloudWindow()
{
    delete ui;
}


