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

#include <qDebug>

static const QStringList supportedFormats({"*.docx", "*.txt"}) ;
const int MAX_FILES_NUMBER = 100;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configHelper(new ConfigHelper(this)),
    settingsDialog(new SettingsDialog(configHelper, this))
{
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());

    configHelper->readSettings();

    dbHelper = new DBHelper(QString("SFM"), QString("sfm.db"), this);

    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    trayIcon->show();

    connect(settingsDialog, &SettingsDialog::pathChanged, this, &MainWindow::rebuildFilesList);
    buildFilesList();

    //    setFilesMonitor();
    //    watcher = new QFileSystemWatcher(this);
    //    watcher->addPaths(QStringList(monitorSet.toList()));
    //    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateIndex(QString)));

    //    fileModel = new QFileSystemModel(this);
    //    fileModel->setRootPath(configHelper->pathModel->item(0)->text());
    //    fileModel->setNameFilters(supportedFormats);
    //    fileModel->setFilter(QDir::Dirs | QDir::Files | QDir::NoDot | QDir::NoDotDot | QDir::NoSymLinks);
    //    qDebug() << "file  model :" << fileModel->rootPath() << fileModel->rootDirectory();
    //    ui->treeView->setModel(fileModel);
    //    ui->treeView->setRootIndex(fileModel->index(fileModel->rootPath()));
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

void MainWindow::setFilesMonitor()//TODO
{
    //may can be ignore while using QFileSystemModel
    /*
       watcher = new QFileSystemWatcher(this);
       watcher->addPaths(QStringList(monitorSet.toList()));
       connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(updateIndex(QString)));
    */
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
    if (settingsDialog->exec() != QDialog::Accepted)
        return;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("关于"),
                       tr("这是一段对智能文件管家的介绍"));
}

void MainWindow::rebuildFilesList()
{
    qDebug() << "start to rebuild monitor list..";
    buildFilesList(true);
}

void MainWindow::buildFilesList(bool renew)
{
    ui->statusBar->showMessage(tr("正在更新监控列表..."));
    if (renew == true)
    {
        filesList.clear();
        dbHelper->cleanFiles();
    }
    if (!(dbHelper->hasIndex()))
    {
        int filesCount = 0;
        for (int i = 0; i < configHelper->pathModel->rowCount(); i++)
        {
            QDirIterator it(configHelper->pathModel->item(i)->text(),
                            //                            supportedFormats,
                            QDir::Files,
                            QDirIterator::Subdirectories);
            while (it.hasNext() && filesCount < MAX_FILES_NUMBER)
            {
                QString thisPath = it.next();
                QFileInfo thisInfo(thisPath);

                File thisFile;
                thisFile.path = thisPath;
                thisFile.name = thisInfo.fileName();
                thisFile.format = thisInfo.suffix();
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
        }
        dbHelper->addFiles(filesList);
    }
    else
        filesList = dbHelper->getFiles();

    //debug
    foreach (auto iter, filesList)
    {
        qDebug() << iter.path;
    }
    qDebug() << "Files num: " << filesList.count();

    ui->statusBar->showMessage(tr("完毕"), 5000);
}

void MainWindow::updateIndex(QString updateFile)
{
    qDebug() << "files changed: " << updateFile;
}

void MainWindow::on_actionAbout_triggered()
{
    about();
}
