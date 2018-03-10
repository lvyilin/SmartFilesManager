#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "settingsdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileupdaterthread.h"

#include "graphwidget.h"
#include "labelswideget.h"
#include "labelgraphwidget.h"
#include "wordlabelwidget.h"
#include "numerictablewidgetitem.h"


#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTime>
#include <QDesktopServices>
#include <QProcess>
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

    //primary init
    configHelper->readSettings();
    settingsDialog = new SettingsDialog(configHelper, this);
    dbHelper = new DBHelper(QString("SFM"), QString("sfm.db"), this);
    connect(this, &MainWindow::quitTask, dbHelper, &DBHelper::abortProgress);
    connect(dbHelper, &DBHelper::dbInterrupted, this, &MainWindow::dbHelperInterrupted);
    connect(dbHelper, &DBHelper::calRelationProgress, this, &MainWindow::showCalRelationProgress);

    analyser = new Analyser(dbHelper, this);
    connect(analyser, &Analyser::interrupted, this, &MainWindow::analyserInterrupted);
    connect(analyser, &Analyser::processFinished, this, &MainWindow::notifyIndexResult);
    connect(analyser, &Analyser::analyseProgress, this, &MainWindow::showAnalyserProgress);

    //view
    setupView();
    ui->pushButtonReload->setIcon(QIcon(":/images/icons/reload.png"));
    ui->pushButtonReload->setToolTip("刷新");
    ui->pushButtonReload->setMaximumSize(ui->comboBoxTreeViewType->height(), ui->comboBoxTreeViewType->height());
    connect(ui->pushButtonReload, &QPushButton::clicked, this, &MainWindow::reloadView);

    //search
    searchBox = new SearchBox(&fileList, this);
    QPushButton *searchButton = new QPushButton(QIcon(":/images/icons/search.png"), QString(), this);
    searchButton->setToolTip("搜索/下一个");
    connect(searchButton, &QPushButton::clicked, searchBox, &QLineEdit::returnPressed);
    searchBox->setPlaceholderText("快速搜索");
    searchBox->setFixedHeight(searchButton->height() * 0.9);
    searchBox->setMaximumWidth(this->width() / 2);
    connect(searchBox, &SearchBox::findFile, this, &MainWindow::treeViewFocus);
    connect(searchBox, &SearchBox::fileNotFound, this, &MainWindow::fileNotFoundMsgBox);
    QWidget *spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->toolBar->addWidget(spacer);
    ui->toolBar->addWidget(searchBox);
    ui->toolBar->addWidget(searchButton);


    //tray
    createTrayIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    trayIcon->show();
    //less important init
    connect(settingsDialog, &SettingsDialog::pathChanged, this, &MainWindow::rebuildFilesList);
    connect(settingsDialog, &SettingsDialog::updateNow, this, &MainWindow::updateFilesList);
    setTrigger();
    //updateFilesList();由用户决定
    if (configHelper->hasLastInterrupted() &&
            configHelper->getInterruptionType() != TerminateInterrupt)
    {
        QMessageBox infoBox;
        infoBox.setText(tr("上次有未完成任务, 是否继续?"));
        infoBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        infoBox.setDefaultButton(QMessageBox::Yes);
        infoBox.setIcon(QMessageBox::Question);
        int ret = infoBox.exec();
        if (ret == QMessageBox::Yes)
        {
            switch (configHelper->getInterruptionType())
            {
            case FileUpdaterInterrupt:
                updateFilesList();
                break;
            case AnalyserInterrupt:
                processWorkList();
                break;
            case DBInterrupt:
                startCalculateRelation();
                break;
            default:
                break;
            }
        }
    }
    //init dict
    /*ToolkitInitThread *toolkitInitThread =  new ToolkitInitThread(this);
    connect(toolkitInitThread, &ToolkitInitThread::startInit, this, &MainWindow::onStartInitToolkit);
    connect(toolkitInitThread, &ToolkitInitThread::finishInit, this, &MainWindow::onFinishInitToolkit);
    connect(this, &MainWindow::quitWorkingThread, toolkitInitThread, &ToolkitInitThread::quit);
    toolkitInitThread->start(QThread::LowPriority);*/

    //draw graph
    drawgraph();
    drawlabelspie();
    drawlabelgraph("计算机");
    drawwordlabel("E:/课件/高级程序设计训练课程材料/高级程序设计训练实验文档命名规范.docx");
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
            trayIcon->showMessage(QCoreApplication::applicationName(), tr("后台运行中"));
        event->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    int fontWidth = ui->tableWidgetRelation->fontMetrics().width("一一一一");
    int scrollBarWidth = fontWidth / 3;
    ui->tableWidgetRelation->setColumnWidth(0, ui->tableWidgetRelation->width() - fontWidth - scrollBarWidth);
    ui->tableWidgetRelation->setColumnWidth(1, fontWidth);
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
    disconnect(relationCalculator, &RelationCalculator::allTasksFinished, this, &MainWindow::notifyRelationFinished);
    configHelper->setInterruptionType(NoInterrupt);

    emit quitTask();

    analyser->quitAll();
    configHelper->close();
    this->thread()->wait(1000);
    dbHelper->close();
    qDebug() << "Safely exit, Bye!";
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
    trayIcon->setToolTip("打开" + QCoreApplication::applicationName());
}

void MainWindow::setTrigger()
{
    if (configHelper->getRunningStrategy() == TimeTrigger)
    {
        if (!triggerTimer)
        {
            triggerTimer = new QTimer(this);
            triggerTimer->setSingleShot(true);
            triggerTimer->setTimerType(Qt::VeryCoarseTimer);
            connect(triggerTimer, SIGNAL(timeout()), this, SLOT(onTriggered()));
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
    configHelper->setFileIndexFinished(false);
    updateFilesList(true);
}

void MainWindow::onTriggered()
{
    processWorkList(true);
}

void MainWindow::processWorkList(bool triggered)
{
    if (triggered)
    {
        //判断是否正在工作中，是，忽略当次触发任务
        if (analyser->getThreadCount() != 0)
        {
            qDebug() << "[processWorkList] triggered but already working, ignored!";
            return;
        }
    }
    if (!configHelper->isFileIndexFinished())
    {
        QMessageBox::information(this, QCoreApplication::applicationName(), tr("文件索引尚未完成."));
        return;
    }
    qDebug() << "【processWorkList】 start process work list...";
    //    ui->statusBar->showMessage(tr("正在处理文件列表..."));

    //获取一次任务最大文件数个文件，再分配到多个线程
    QVector<File> workList;
    dbHelper->getWorkList(workList, WORKLIST_SIZE * MAX_WORKLIST_NUM);
    for (size_t i = 0; i < MAX_WORKLIST_NUM; ++i)
    {
        if (workList.isEmpty())
        {
            if (i == 0)
            {
                notifyIndexResult(0, 0);
            }
            break;
        }
        QList<File> singleWorkList;
        for (size_t j = 0; !workList.isEmpty() && j < WORKLIST_SIZE; ++j)
        {
            singleWorkList.append(workList.takeLast());
        }
        qDebug() << "[processWorkList] single worklist count: " << singleWorkList.count();
        analyser->processFileList(singleWorkList);
    }
}

QString MainWindow::fileSizeHumanReadable(qint64 num)
{
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while (num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum((float)num, 'f', 2) + " " + unit;
}

QMap<QString, QStringList> &MainWindow::getLabelFilesMap()
{

    return fileTreeFieldModel->labelFilesMap;
}

void MainWindow::updateFilesList(bool renew)
{
    ui->statusBar->showMessage(tr("正在更新文件列表..."));
    if (renew == true)
    {
        dbHelper->cleanFiles();
    }
    QStringList pathList;
    for (int i = 0; i < configHelper->pathModel->rowCount(); i++)
    {
        pathList << configHelper->pathModel->item(i)->text();
    }

    FileUpdaterThread *updateThread =  new FileUpdaterThread(dbHelper, FORMATS_FILTER, pathList, this);
    connect(updateThread, &FileUpdaterThread::resultReady, this, &MainWindow::showUpdaterResult);
    connect(updateThread, &FileUpdaterThread::findFilesProgress, this, &MainWindow::showUpdaterProgress);
    connect(updateThread, &FileUpdaterThread::startDbProgress, this, &MainWindow::showUpdaterDbProgress);
    connect(updateThread, &FileUpdaterThread::finished, this, &MainWindow::fileUpdaterFinished);
    connect(updateThread, &FileUpdaterThread::finished, this, &MainWindow::setupView);
    connect(updateThread, &FileUpdaterThread::finished, &QObject::deleteLater);
    connect(updateThread, &FileUpdaterThread::aborted, this, &MainWindow::fileUpdaterInterrupted);
    //    connect(this, &MainWindow::fileUpdaterWait, updateThread, &FileUpdaterThread::wait);
    connect(this, &MainWindow::quitTask, updateThread, &FileUpdaterThread::abortProgress);
    updateThread->start();
}

void MainWindow::fileUpdaterInterrupted()
{
    configHelper->setInterruptionType(FileUpdaterInterrupt);
}

void MainWindow::fileUpdaterFinished()
{
    configHelper->setFileIndexFinished(true);
}

void MainWindow::analyserInterrupted()
{
    configHelper->setInterruptionType(AnalyserInterrupt);
}

void MainWindow::dbHelperInterrupted()
{
    configHelper->setInterruptionType(DBInterrupt);
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

void MainWindow::showAnalyserProgress(int num)
{
    ui->statusBar->showMessage(tr("正在分析文件...已完成 %1").arg(num));
}

void MainWindow::showCalRelationProgress(int num, int total)
{
    ui->statusBar->showMessage(tr("正在保存文件关系计算结果...(%1/%2)").arg(num).arg(total));
}

void MainWindow::showStartCalRelation()
{
    ui->statusBar->showMessage(tr("正在计算文件关系..."));
}

void MainWindow::notifyIndexResult(int success, int fail)
{
    ui->statusBar->showMessage(tr("文件索引建立完成."));
    if (configHelper->isAutoCalRelation())
    {
        calculateRelationSeparately = false;
        startCalculateRelation();
    }
    else
    {
        trayIcon->showMessage(QCoreApplication::applicationName(),
                              tr("索引建立完成, 成功%1个，失败%2个, 打开主页面以查看结果")
                              .arg(success)
                              .arg(fail));
        connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showWindowAndDisconnect()));
    }
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

void MainWindow::onStartInitToolkit()
{
    ui->statusBar->showMessage(tr("正在初始化词典..."));
}

void MainWindow::onFinishInitToolkit()
{
    ui->statusBar->showMessage(tr("词典初始化完成."));
}

void MainWindow::setupView()
{
    ui->tableWidgetAttr->setColumnCount(2);
    ui->tableWidgetAttr->setHorizontalHeaderLabels(QStringList() << "文件属性" << "值");
    ui->tableWidgetAttr->horizontalHeader()->setVisible(true);

    int fontWidth = ui->tableWidgetAttr->fontMetrics().width("一一一一一");
    ui->tableWidgetAttr->setColumnWidth(0, fontWidth);

    ui->tableWidgetRelation->setColumnCount(2);
    ui->tableWidgetRelation->setHorizontalHeaderLabels(QStringList() << "关联文件" << "关系度");
    ui->tableWidgetRelation->horizontalHeader()->setVisible(true);
    connect(ui->tableWidgetRelation, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(focusFile()));

    int scrollBarWidth = fontWidth / 3.75;
    ui->tableWidgetRelation->setColumnWidth(0, ui->tableWidgetRelation->width() - fontWidth - scrollBarWidth);
    ui->tableWidgetRelation->setColumnWidth(1, fontWidth);

    QList<int> idList;
    connect(ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showFileContextMenu(QPoint)));
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_actionOpenFile_triggered()));
    connect(ui->treeWidgetField, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showFieldContextMenu(QPoint)));

    dbHelper->getAllFiles(fileList, idList);
    //    ui->treeView->hide();
    if (fileTreeFormatModel == nullptr)
    {
        fileTreeFormatModel = new FileTreeModel(fileList, dbHelper, this);
        fileTreeFormatModel->setupTypeModelData();
        ui->treeView->setModel(fileTreeFormatModel);
    }
    else
    {
        FileTreeModel *anotherModel = new FileTreeModel(fileList, dbHelper, this);
        anotherModel->setupTypeModelData();
        ui->treeView->setModel(anotherModel);
        delete fileTreeFormatModel;
        fileTreeFormatModel = anotherModel;
    }

    if (fileTreeFieldModel == nullptr)
    {
        fileTreeFieldModel = new FileTreeModel(fileList, dbHelper, this);
        fileTreeFieldModel->setupFieldModelData();
    }

    ui->treeView->show();
}

void MainWindow::reloadView()
{
    QList<int> idList;
    dbHelper->getAllFiles(fileList, idList);
    int idx = ui->comboBoxTreeViewType->currentIndex();
    //    ui->treeView->hide();
    FileTreeModel *anotherModel = new FileTreeModel(fileList, dbHelper, this);
    if (idx == 0)
    {
        anotherModel->setupTypeModelData();
        ui->treeView->setModel(anotherModel);
        delete fileTreeFormatModel;
        fileTreeFormatModel = anotherModel;
    }
    else if (idx == 1)
    {
        anotherModel->setupFieldModelData();
        ui->treeView->setModel(anotherModel);
        delete fileTreeFieldModel;
        fileTreeFieldModel = anotherModel;
    }

    on_treeView_clicked(anotherModel->index(0, 0));
}

void MainWindow:: treeViewFocus(const QString &str, bool byPath)
{
    int idx = ui->comboBoxTreeViewType->currentIndex();
    FileTreeModel *model;
    if (idx == 0)
        model = fileTreeFormatModel;
    else if (idx == 1)
        model = fileTreeFieldModel;
    else;

    int role = byPath ? Qt::ToolTipRole
               : Qt::DisplayRole;
    QModelIndexList idxs = model->match(
                               model->index(0, 0),
                               role,
                               QVariant(str),
                               1,
                               Qt::MatchRecursive);
    if (idxs.isEmpty())
    {
        fileNotFoundMsgBox();
        return;
    }
    ui->treeView->setCurrentIndex(idxs[0]);
    on_treeView_clicked(idxs[0]);
}

void MainWindow::fileNotFoundMsgBox()
{
    QMessageBox::information(this, QCoreApplication::applicationName(), "找不到文件");
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    ui->treeWidgetField->clear();
    ui->listWidgetKw->clear();
    ui->listWidgetLabel->clear();
    ui->tableWidgetAttr->setRowCount(0);
    ui->tableWidgetRelation->setRowCount(0);

    FileItem *item = static_cast<FileItem *>(index.internalPointer());
    QString path = item->data(1).toString();
    if (path.isEmpty())
        return;
    FileResult fr;
    dbHelper->getFileResultByPath(path, fr);

    //attribute view
    ui->tableWidgetAttr->setRowCount(6);
    int fontHeight = ui->tableWidgetAttr->fontMetrics().height();
    for (int i = 0; i < 6; ++i)
        ui->tableWidgetAttr->setRowHeight(i, fontHeight * 1.2);

    QTableWidgetItem *witem;
    ui->tableWidgetAttr->setItem(0, 0, new QTableWidgetItem(QString("名称")));
    witem = new QTableWidgetItem(fr.file.name);
    witem->setToolTip(fr.file.name);
    ui->tableWidgetAttr->setItem(0, 1, witem);

    ui->tableWidgetAttr->setItem(1, 0, new QTableWidgetItem(QString("类型")));
    witem = new QTableWidgetItem(fr.file.format);
    ui->tableWidgetAttr->setItem(1, 1, witem);

    ui->tableWidgetAttr->setItem(2, 0, new QTableWidgetItem(QString("路径")));
    witem = new QTableWidgetItem(fr.file.path);
    witem->setToolTip(fr.file.path);
    ui->tableWidgetAttr->setItem(2, 1, witem);

    ui->tableWidgetAttr->setItem(3, 0, new QTableWidgetItem(QString("大小")));
    witem = new QTableWidgetItem(fileSizeHumanReadable(fr.file.size));
    ui->tableWidgetAttr->setItem(3, 1, witem);

    ui->tableWidgetAttr->setItem(4, 0, new QTableWidgetItem(QString("创建日期")));
    witem = new QTableWidgetItem(
        fr.file.createTime.toString(Qt::SystemLocaleLongDate));
    witem->setToolTip(fr.file.createTime.toString(Qt::SystemLocaleLongDate));
    ui->tableWidgetAttr->setItem(4, 1, witem);

    ui->tableWidgetAttr->setItem(5, 0, new QTableWidgetItem(QString("修改日期")));
    witem = new QTableWidgetItem(
        fr.file.modifyTime.toString(Qt::SystemLocaleLongDate));
    witem->setToolTip(fr.file.modifyTime.toString(Qt::SystemLocaleLongDate));
    ui->tableWidgetAttr->setItem(5, 1, witem);

    //field view
    QList<Label> fieldLabels;
    QList<QTreeWidgetItem *> fieldItems;
    QList<QTreeWidgetItem *> topLevelItems;
    for (Label &label : fr.labels)
    {
        if (label.type == "field")
            fieldLabels << label;
    }
    for (int i = 0; i < fieldLabels.count(); ++i)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << fieldLabels[i].name);
        fieldItems << item;
        if (fieldLabels[i].level == 1)
            topLevelItems << item;
        //            root->addChild(item);
    }
    for (int i = 0; i < fieldItems.count(); ++i)
    {
        if (fieldLabels[i].level != 1)
        {
            for (int j = 0; j < fieldLabels.count(); ++j)
            {
                if (fieldLabels[i].parentName == fieldLabels[j].name)
                {
                    fieldItems[j]->addChild(fieldItems[i]);
                }
            }
        }
    }

    ui->treeWidgetField->addTopLevelItems(topLevelItems);
    ui->treeWidgetField->expandAll();

    //keyword view

    QMapIterator<QString, double> iter(fr.keywords);
    while (iter.hasNext())
    {
        iter.next();
        ui->listWidgetKw->addItem(new QListWidgetItem(iter.key()));
    }

    //label view
    for (Label &label : fr.labels)
    {
        ui->listWidgetLabel->addItem(new QListWidgetItem(label.name));
    }
    //relation view
    ui->tableWidgetRelation->setRowCount(fr.relations.size());
    for (int i = 0; i < fr.relations.size(); ++i)
        ui->tableWidgetRelation->setRowHeight(i, fontHeight * 1.2);

    for (int i = 0; i < fr.relations.size(); ++i)
    {
        QTableWidgetItem *rlItem = new QTableWidgetItem(fr.relations[i].file.name);
        rlItem->setToolTip(fr.relations[i].file.name);
        ui->tableWidgetRelation->setItem(i, 0, rlItem);
        double totalRelation = fr.relations[i].keywordDegree * KEYWORD_RELATION_WEIGHT
                               + fr.relations[i].labelDegree * LABEL_RELATION_WEIGHT
                               + fr.relations[i].attributeDegree * ATTRIBUTE_RELATION_WEIGHT;
        ui->tableWidgetRelation->setItem(i, 1, new NumericTableWidgetItem(QString("%1%").arg(totalRelation * 100, 0, 'f', 1)));
    }
    ui->tableWidgetRelation->sortItems(1, Qt::DescendingOrder);

    int fontWidth = ui->tableWidgetRelation->fontMetrics().width("一一一一");
    int scrollBarWidth = fontWidth / 3;
    ui->tableWidgetRelation->setColumnWidth(0, ui->tableWidgetRelation->width() - fontWidth - scrollBarWidth);
    ui->tableWidgetRelation->setColumnWidth(1, fontWidth);
}

void MainWindow::startCalculateRelation()
{
    dbHelper->cleanRelations();
    if (!relationCalculator)
    {
        relationCalculator = new RelationCalculator(dbHelper, this);
        connect(relationCalculator, &RelationCalculator::allTasksFinished, this, &MainWindow::notifyRelationFinished);
        connect(relationCalculator, &RelationCalculator::startTask, this, &MainWindow::showStartCalRelation);
        connect(dbHelper, &DBHelper::finishSaveFileResult, relationCalculator, &RelationCalculator::newTaskOk);
        relationCalculator->start();
    }
    else if (relationCalculator->isFinished())
    {
        relationCalculator->start();
    }
    else
    {
        QMessageBox::information(this, QCoreApplication::applicationName(), tr("当前计算任务尚未完成."));
    }
}

void MainWindow::notifyRelationFinished()
{
    ui->statusBar->showMessage(tr("文件关系计算完成."));
    QString finishMsg;
    if (!calculateRelationSeparately)
    {
        finishMsg = "本次任务完成，打开界面以查看结果";
    }
    else
    {
        finishMsg = "文件关系建立完成";
    }
    trayIcon->showMessage(QCoreApplication::applicationName(), finishMsg);
}

void MainWindow::on_actionIndex_triggered()
{
    onStartInitToolkit();
    processWorkList();
}

void MainWindow::on_actionBuildRelation_triggered()
{
    startCalculateRelation();
}

void MainWindow::on_actionQuickSearch_triggered()
{
    searchBox->setFocus();
}

void MainWindow::on_actionAdvancedSearch_triggered()
{
    if (searchDialog == nullptr)
        searchDialog = new SearchDialog(dbHelper, this);
    searchDialog->show();
}

void MainWindow::on_actionRefrashFiles_triggered()
{
    rebuildFilesList();
}

void MainWindow::on_actionOpenFile_triggered()
{
    QString path = ui->treeView->currentIndex().data(Qt::ToolTipRole).toString();
    if (!path.isEmpty())
        QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void MainWindow::on_actionOpenFolder_triggered()
{
    QString path = ui->treeView->currentIndex().data(Qt::ToolTipRole).toString();
    /*QFileInfo info(path);
    if (path.isEmpty())
        return;

    if (!info.isDir())
    {
        path = info.absolutePath();
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));*/

    const QString explorer = "explorer.exe";
    if (explorer.isEmpty())
    {
        QMessageBox::warning(this,
                             tr("Launching Windows Explorer failed"),
                             tr("Could not find explorer.exe in path to launch Windows Explorer."));
        return;
    }
    QString param;
    if (!QFileInfo(path).isDir())
        param = QLatin1String("/select,");
    param += QDir::toNativeSeparators(path);
    QString command = explorer + " " + param;
    QProcess::startDetached(command);
}

void MainWindow::on_actionArrange_triggered()
{
    FileItem *item =  static_cast<FileItem *>(ui->treeView->currentIndex().internalPointer());
    if (item->childCount() == 0) return;
    const QString directoryName =
        QFileDialog::getExistingDirectory(this,
                                          tr("选择保存路径"),
                                          QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (directoryName.isEmpty())
    {
        return;
    }
    QString pathPrefix = QDir::toNativeSeparators(directoryName) +
                         QDir::separator() +
                         item->data(0).toString() +
                         QDir::separator();
    if (!QDir(pathPrefix).exists())
    {
        QDir().mkpath(pathPrefix);
    }
    for (int i = 0; i < item->childCount(); ++i)
    {
        QString path = item->child(i)->data(1).toString();
        QFileInfo info(path);
        if (QFileInfo(path).exists())
        {
            QFile::copy(path, pathPrefix + info.fileName());
        }
    }
    QDesktopServices::openUrl(QUrl::fromLocalFile(pathPrefix));
}

void MainWindow::showFileContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeView->mapToGlobal(pos);
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid())
        return;
    if (index.data(Qt::ToolTipRole).toString().isEmpty())
    {
        FileItem *item =  static_cast<FileItem *>(index.internalPointer());
        if (item->childCount() == 0) return;
        if (!item->child(0)->data(1).toString().isEmpty())
        {
            if (fileTreeMenu != nullptr)
                delete fileTreeMenu;
            fileTreeMenu = new QMenu(this);
            fileTreeMenu->addAction(ui->actionArrange);
            fileTreeMenu->exec(globalPos);
        }
    }
    else
    {
        if (fileTreeMenu != nullptr)
            delete fileTreeMenu;
        fileTreeMenu = new QMenu(this);
        fileTreeMenu->addAction(ui->actionOpenFile);
        fileTreeMenu->addAction(ui->actionOpenFolder);
        fileTreeMenu->exec(globalPos);
    }
}

void MainWindow::showFieldContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->treeWidgetField->mapToGlobal(pos);
    QModelIndex index = ui->treeWidgetField->indexAt(pos);
    if (!index.isValid())
        return;

    if (fieldMenu != nullptr)
        delete fieldMenu;
    fieldMenu = new QMenu(this);
    fieldMenu->addAction(ui->actionFieldFile);
    fieldMenu->exec(globalPos);
}

void MainWindow::focusFile()
{
    QString curPath = ui->tableWidgetRelation->currentIndex().data().toString();
    treeViewFocus(curPath, false);
}


void MainWindow::drawwordlabel(QString path)
{
    QWidget *widget = new QWidget(this);
    wordlabelwidget *wordlabelwidget_ = new wordlabelwidget(this, dbHelper, configHelper);

    wordlabelwidget_->setpath(path);

    QHBoxLayout *wordlabelLayout = new QHBoxLayout(widget);
    wordlabelLayout->addWidget(wordlabelwidget_, 3);
    QListWidget *wordlabelListWidget = new QListWidget(widget);
    wordlabelLayout->addWidget(wordlabelListWidget, 1);
    widget->setLayout(wordlabelLayout);
    wordlabelwidget_->list = wordlabelListWidget;
    ui->tabWidget_2->addTab(widget, "标签词云展示");
    // ui->word_cloud_4 = wordcloudwidget_;
}

void MainWindow::drawgraph()
{
    graphwidget *graphwidget_ = new graphwidget(this, dbHelper, configHelper);
    ui->tabWidget_2->addTab(graphwidget_, "知识图谱类型视图");
    // ui->graph_view_2 = graphwidget_;
}

void MainWindow::drawlabelspie()
{
    QWidget *widget = new QWidget(this);
    labelswideget *labelswideget_ = new labelswideget(getLabelFilesMap(), this, dbHelper, configHelper);
    QHBoxLayout *pieLayout = new QHBoxLayout(widget);
    pieLayout->addWidget(labelswideget_, 3);
    QTreeWidget *pieTreeWidget = new QTreeWidget(widget);
    pieLayout->addWidget(pieTreeWidget, 1);
    widget->setLayout(pieLayout);
    labelswideget_->tree = pieTreeWidget;
    ui->tabWidget_2->addTab(widget, "标签饼状图视图");
}

void MainWindow::drawlabelgraph(QString name)
{
    labelgraphwidget *labelgraphwidget_ = new labelgraphwidget(this, dbHelper, configHelper);
    labelgraphwidget_->setname(name);
    ui->tabWidget_2->addTab(labelgraphwidget_, "指定标签下的知识图谱类型视图");
}

void MainWindow::on_comboBoxTreeViewType_currentIndexChanged(int index)
{
    if (fileTreeFormatModel == nullptr)
    {
        fileTreeFormatModel = new FileTreeModel(fileList, dbHelper, this);
        if (index == 0)
            fileTreeFormatModel->setupTypeModelData();
        else if (index == 1)
            fileTreeFormatModel->setupFieldModelData();
        ui->treeView->setModel(fileTreeFormatModel);
    }
    else
    {
        FileTreeModel *anotherModel = new FileTreeModel(fileList, dbHelper, this);
        if (index == 0)
            anotherModel->setupTypeModelData();
        else if (index == 1)
            anotherModel->setupFieldModelData();
        ui->treeView->setModel(anotherModel);
        delete fileTreeFormatModel;
        fileTreeFormatModel = anotherModel;
    }
}

void MainWindow::on_actionArrangeInfo_triggered()
{
    QMessageBox::information(this, QCoreApplication::applicationName(), "请从文件浏览窗格任选一个分类，单击右键进行整理。");
}

void MainWindow::on_actionFieldFile_triggered()
{

}
