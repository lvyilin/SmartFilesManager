﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include <QDirIterator>
#include <QSet>
#include <QFileInfo>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include "utils.h"
#include "confighelper.h"
#include "dbhelper.h"
#include "analyser.h"

#include "filetreemodel.h"
#include "graphwidget.h"
#include "labelswideget.h"
#include "relationcalculator.h"
#include "searchdialog.h"
#include "searchbox.h"

#include "labelgraphwidget.h"
#include "wordlabelwidget.h"
#include "graphwidget.h"
#include "labelswideget.h"


class SettingsDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void quitTask();
    void fileUpdaterWait(unsigned long time);

public slots:
    void treeViewFocus(const QString &str, bool byPath = true);
    void fileNotFoundMsgBox();

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void openSettings();
    void about();
    void rebuildFilesList();
    void onTriggered();
    void showWindowAndDisconnect();
    void on_actionExit_triggered();
    void on_actionSettings_triggered();
    void on_actionAbout_triggered();
    void showUpdaterResult(const QString &res);
    void showUpdaterProgress(int num);
    void showAnalyserProgress(int num);
    void showCalRelationProgress(int num, int total);
    void showStartCalRelation();

    void showUpdaterDbProgress();
    void notifyIndexResult(int success, int fail);
    void updateFilesList(bool renew = false);

    void fileUpdaterInterrupted();
    void fileUpdaterFinished();
    void analyserInterrupted();
    void dbHelperInterrupted();

    void onStartInitToolkit();
    void onFinishInitToolkit();

    void setupView();
    void reloadView();

    void on_treeView_clicked(const QModelIndex &index);
    //    void drawgraph();
    void drawlabelspie();

    void notifyRelationFinished();

    void on_actionIndex_triggered();

    void on_actionBuildRelation_triggered();

    void on_actionQuickSearch_triggered();

    void on_actionAdvancedSearch_triggered();

    void on_actionRefrashFiles_triggered();

    void on_actionOpenFile_triggered();
    void on_actionOpenFolder_triggered();
    void showFileContextMenu(const QPoint &pos);
    void showFieldContextMenu(const QPoint &pos);
    void focusFile();

    void on_comboBoxTreeViewType_currentIndexChanged(int index);

    void on_actionArrange_triggered();

    void on_actionArrangeInfo_triggered();

    void on_actionFieldFile_triggered();

    void on_actionImagine_triggered();

    void updateGraph();
    void updatePie();
    void updateFieldTreeModel();

private:
    Ui::MainWindow *ui;
    bool calculateRelationSeparately = true;

    void readyQuit();
    void createTrayIcon();
    void setTrigger();
    void processWorkList(bool triggered = false);
    void startCalculateRelation();
    QString fileSizeHumanReadable(qint64 num);

    QMap<QString, QStringList> &getLabelFilesMap();

    ConfigHelper *configHelper;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    SettingsDialog *settingsDialog;
    DBHelper *dbHelper;
    QTimer *triggerTimer;
    Analyser *analyser;
    FileTreeModel *fileTreeFormatModel = nullptr;
    FileTreeModel *fileTreeFieldModel = nullptr;
    RelationCalculator *relationCalculator = nullptr;

    SearchBox *searchBox;
    QList<File> fileList;
    SearchDialog *searchDialog = nullptr;

    QMenu *fileTreeMenu = nullptr;
    QMenu *fieldMenu = nullptr;
    labelgraphwidget *labelgraphwidget_ = nullptr;
    wordlabelwidget *wordlabelwidget_ = nullptr;
    graphwidget *graphwidget_ ;
    labelswideget *labelswideget_;
};

#endif // MAINWINDOW_H
