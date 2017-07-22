#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settingsDialog(new SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());
    init();
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
    if (trayIcon->isVisible()) {
        hide();
        trayIcon->showMessage(tr("智能文件管家"),tr("后台运行中"));
        event->ignore();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        show();
        break;
    default:
        ;
    }
}

void MainWindow::init()
{
    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    trayIcon->show();
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(tr("显示主窗口"),this,&QWidget::showNormal);
    trayIconMenu->addAction(tr("打开设置"),settingsDialog,&QDialog::exec);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("退出程序"),this,&QCoreApplication::quit);

    trayIcon = new QSystemTrayIcon(QIcon(":/images/icons/tray.png"),this);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("打开智能文件管家"));
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionSettings_triggered()
{
    settingsDialog->exec();
}
