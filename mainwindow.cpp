#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

/*TODO
 * updateIndex() 优化：去除已包含过的子目录
 * 单次处理文件数目 可设置化
 */

#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileupdaterthread.h"

#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTime>

#include <qDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configHelper(new ConfigHelper(this)),
    triggerTimer(Q_NULLPTR)
{
    //ui
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());
    connect(ui->processButton, SIGNAL(clicked(bool)), this, SLOT(processWorkList()));
    //primary init
    configHelper->readSettings();
    settingsDialog = new SettingsDialog(configHelper, this);
    dbHelper = new DBHelper(QString("SFM"), QString("sfm.db"), this);
    if (configHelper->isFirstTimeUsing())
        dbHelper->initLabels();
    analyser = new Analyser(dbHelper, this);

    connect(analyser, &Analyser::processFinished, this, &MainWindow::notifyResult);

    //tray
    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    trayIcon->show();
    //less important init
    connect(settingsDialog, &SettingsDialog::pathChanged, this, &MainWindow::rebuildFilesList);
    connect(settingsDialog, &SettingsDialog::updateNow, this, &MainWindow::updateFilesList);
    setTrigger();
    //updateFilesList();由用户决定
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        hide();
        if (configHelper->isFirstTimeUsing())
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
        showNormal();
        break;
    default:
        ;
    }
}

void MainWindow::readyQuit()
{
    qDebug() << "Safely exit, Bye!";
    dbHelper->close();
    QCoreApplication::quit();
}


void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(tr("显示主窗口"), this, &QWidget::showNormal);
    trayIconMenu->addAction(tr("打开设置"), this, &MainWindow::openSettings);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("退出程序"), this, &MainWindow::readyQuit);

    trayIcon = new QSystemTrayIcon(QIcon(":/images/icons/tray.png"), this);

    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("打开智能文件管家"));
}

void MainWindow::setTrigger()
{
    if (configHelper->getRunningStrategy() == ConfigHelper::TimeTrigger)
    {
        if (!triggerTimer)
        {
            triggerTimer = new QTimer(this);
            triggerTimer->setSingleShot(true);
            triggerTimer->setTimerType(Qt::VeryCoarseTimer);
            connect(triggerTimer, SIGNAL(timeout()), this, SLOT(processWorkList()));
        }
        else if (triggerTimer->isActive())
            triggerTimer->stop();

        int itv = QTime::currentTime().msecsTo(configHelper->getTimeTriggerPoint());
        if (itv < 0)
        {
            itv = QTime::currentTime().msecsTo(QTime(23, 59, 59, 999)) +
                  QTime(0, 0).msecsTo(configHelper->getTimeTriggerPoint());
        }
        triggerTimer->start(itv);

        qDebug() << "[setTrigger] time trigger after" << QTime::fromMSecsSinceStartOfDay(itv).toString();
    }
    else
    {
        //CPU trigger here
    }
}

void MainWindow::on_actionExit_triggered()
{
    readyQuit();
}

void MainWindow::on_actionSettings_triggered()
{
    openSettings();
}

void MainWindow::openSettings()
{
    if (settingsDialog->exec() == QDialog::Accepted)
    {
        setTrigger();//重新设置触发器
    }
    if (this->isHidden())
    {
        this->showMinimized();
        this->hide();
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于"),
                       tr("这是一段对智能文件管家的介绍"));
}

void MainWindow::rebuildFilesList()
{
    qDebug() << "[rebuildFilesList] start to rebuild files list..";
    updateFilesList(true);
}

void MainWindow::processWorkList()
{
    qDebug() << "【processWorkList】 start process work list...";
    ui->statusBar->showMessage(tr("正在处理文件列表..."));
    //每种格式处理固定个数文件
    foreach (QString format, analyser->getSupportedFormatsList())
    {
        workList = dbHelper->getWorkList(format, 1000);
        if (workList.isEmpty())
        {
            qDebug() << "[processWorkList] worklist is empty.";
            notifyResult(0, 0);
            break;
        }
        //debug-------------
        qDebug() << "[processWorkList] work list count: " << workList.count();
        /*foreach (auto iter, workList)
        {
            qDebug() << iter.path;
        }*/
        //------------------
        analyser->processFileList(workList);
    }
}


void MainWindow::updateFilesList(bool renew)
{
    ui->statusBar->showMessage(tr("正在更新文件列表..."));
    if (renew == true)
    {
        dbHelper->cleanFiles();
    }
    QStringList formatList = analyser->getSupportedFormatsFilter();
    QStringList pathList;
    for (int i = 0; i < configHelper->pathModel->rowCount(); i++)
    {
        pathList << configHelper->pathModel->item(i)->text();
    }

    FileUpdaterThread *updateThread =  new FileUpdaterThread(dbHelper, formatList, pathList, this);
    connect(updateThread, &FileUpdaterThread::resultReady, this, &MainWindow::showUpdaterResult);
    connect(updateThread, &FileUpdaterThread::findFilesProgress, this, &MainWindow::showUpdaterProgress);
    connect(updateThread, &FileUpdaterThread::startDbProgress, this, &MainWindow::showUpdaterDbProgress);
    connect(updateThread, &FileUpdaterThread::finished, &QObject::deleteLater);
    updateThread->start();
}

void MainWindow::showUpdaterResult(const QString &res)
{
    ui->statusBar->showMessage(res);
}

void MainWindow::showUpdaterDbProgress()
{
    ui->statusBar->showMessage(tr("正在添加文件到数据库..."));
}

void MainWindow::showUpdaterProgress(int num)
{
    ui->statusBar->showMessage(tr("正在添加文件...已找到 %1").arg(num));
}

void MainWindow::notifyResult(int success, int fail)
{
    ui->statusBar->showMessage(tr("文件列表处理完成."));
    trayIcon->showMessage(tr("智能文件管家"),
                          tr("索引建立完成, 成功%1个，失败%2个, 打开主页面以查看结果")
                          .arg(success)
                          .arg(fail));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showWindowAndDisconnect()));
}

void MainWindow::showWindowAndDisconnect()
{
    disconnect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showWindowAndDisconnect()));
    show();
}

void MainWindow::on_actionAbout_triggered()
{
    about();
}

