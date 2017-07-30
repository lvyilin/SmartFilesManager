﻿#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

/*TODO
 * updateIndex() 优化：去除已包含过的子目录
 */

#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

#include <qDebug>

static const QStringList supportedFormats({"*.docx", "*.txt"}) ;
const int MAX_FILES_NUMBER = 100;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configHelper(new ConfigHelper(this)),
    settingsDialog(Q_NULLPTR)
{
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());

    configHelper->readSettings();

    dbHelper = new DBHelper(QString("SFM"), QString("sfm.db"), this);

    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    trayIcon->show();

    buildMonitorSet();
    //    setFilesMonitor();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    #ifdef Q_OS_OSX
    if (!event->spontaneous() || !isVisible())
    {
        return;
    }
    #endif
    if (trayIcon->isVisible())
    {
        hide();
        trayIcon->showMessage(tr("智能文件管家"), tr("后台运行中"));
        event->ignore();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        show();
        break;
    default:
        ;
    }
}

void MainWindow::reallyQuit()
{
    dbHelper->close();
    QCoreApplication::quit();
}


void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(tr("显示主窗口"), this, &QWidget::showNormal);
    trayIconMenu->addAction(tr("打开设置"), this, &MainWindow::openSettings);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("退出程序"), this, &MainWindow::reallyQuit);

    trayIcon = new QSystemTrayIcon(QIcon(":/images/icons/tray.png"), this);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("打开智能文件管家"));
}

void MainWindow::setFilesMonitor()
{
    watcher->addPaths(monitorSet.toList());
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateIndex(QString)));

}

void MainWindow::on_actionExit_triggered()
{
    reallyQuit();
}

void MainWindow::on_actionSettings_triggered()
{
    openSettings();
}

void MainWindow::openSettings()
{
    if (!settingsDialog)
        settingsDialog = new SettingsDialog(configHelper, this);

    if (settingsDialog->exec() != QDialog::Accepted)
        return;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于"),
                       tr("这是一段对智能文件管家的介绍"));
}

void MainWindow::buildMonitorSet()
{
    ui->statusBar->showMessage(tr("正在添加文件至监控列表..."));
    if (!(dbHelper->hasIndex()))
    {
        int filesCount = 0;
        for (int i = 0; i < configHelper->pathModel->rowCount(); i++)
        {
            QDirIterator it(configHelper->pathModel->item(i)->text(),
                            supportedFormats,
                            QDir::Files,
                            QDirIterator::Subdirectories);
            while (it.hasNext() && filesCount < MAX_FILES_NUMBER)
            {
                monitorSet << it.next();
                ++filesCount;
            }
            if (MAX_FILES_NUMBER == filesCount)
                break;
        }
        if (MAX_FILES_NUMBER == filesCount)
        {
            QMessageBox::warning(this,
                                 tr("操作中断"),
                                 tr("超过文件数限制%1，请前往\"设置\"修改监控文件夹并缩小监控范围。").arg(MAX_FILES_NUMBER));
        }
        dbHelper->addFiles(monitorSet);
    }
    else
        monitorSet = dbHelper->getFiles();

    //debug
    foreach (auto iter, monitorSet)
    {
        qDebug() << iter;
    }
    qDebug() << "Files num: " << monitorSet.count();

    ui->statusBar->showMessage(tr("完毕"), 5000);
}

void MainWindow::updateIndex(QString needUpdatePath)
{
    qDebug() << "need update path:" << needUpdatePath;
}

void MainWindow::on_actionAbout_triggered()
{
    about();
}
