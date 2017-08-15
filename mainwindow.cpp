#if defined(_MSC_VER) && (_MSC_VER >= 1600)
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
#include <QTime>

#include <qDebug>

const int MAX_FILES_NUMBER = 500;

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
    //tray
    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    trayIcon->show();
    //less important init
    connect(settingsDialog, &SettingsDialog::pathChanged, this, &MainWindow::rebuildFilesList);
    //    connect(this, &MainWindow::onFinishedWorkList, this, &MainWindow::setTrigger);若运行过快会导致因在同时刻而多次触发
    setTrigger();
    updateFilesList();
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

void MainWindow::reallyQuit()
{
    qDebug() << "Save exit, Bye!";
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
    reallyQuit();
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
    qDebug() << "【Triggered!】 start process work list...";
    workList = dbHelper->getWorkList("docx", 100);
    qDebug() << "[processWorkList] work list: ";
    foreach (auto iter, workList)
    {
        qDebug() << iter.path << iter.isFinished;
    }

    int successCount, failCount;
    successCount = failCount = 0;
    foreach (File file, workList)
    {
        if (analyser->processFile(file))
            ++successCount;
        else ++failCount;
    }

    notifyResult(successCount, failCount);
    emit onFinishedWorkList();
}


void MainWindow::updateFilesList(bool renew)
{
    ui->statusBar->showMessage(tr("正在更新文件列表..."));
    if (renew == true)
    {
        filesList.clear();
        dbHelper->cleanFiles();
    }
    int filesCount = 0;
    for (int i = 0; i < configHelper->pathModel->rowCount(); i++)
    {
        QDirIterator it(configHelper->pathModel->item(i)->text(),
                        analyser->getSupportedFormatsList(),
                        QDir::Files,
                        QDirIterator::Subdirectories);
        while (it.hasNext() && filesCount < MAX_FILES_NUMBER)
        {
            QString thisPath = it.next();
            QFileInfo thisInfo(thisPath);

            File thisFile;
            thisFile.path = thisPath;
            thisFile.name = thisInfo.fileName();
            thisFile.format = thisInfo.suffix().toLower();
            thisFile.createTime = thisInfo.created();
            thisFile.modifyTime = thisInfo.lastModified();
            thisFile.size = thisInfo.size();
            thisFile.isFinished = false;

            filesList << thisFile;
            ++filesCount;
        }
        if (MAX_FILES_NUMBER == filesCount)
            break;
    }
    if (MAX_FILES_NUMBER == filesCount)
    {
        QMessageBox::warning(this,
                             tr("操作中断"),
                             tr("超过单文件夹文件数限制%1，请前往\"设置\"修改文件夹路径以缩小范围。").arg(MAX_FILES_NUMBER));
        ui->statusBar->showMessage(tr("添加文件失败"), 5000);
    }
    else
    {
        dbHelper->addFiles(filesList);

        foreach (auto iter, filesList)
        {
            qDebug() << "[updateFilesList] " << iter.path;
        }
        qDebug() << "[updateFilesList] files num: " << filesList.count();

        ui->statusBar->showMessage(tr("完毕"), 5000);
    }
}

void MainWindow::notifyResult(int success, int fail)
{
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
