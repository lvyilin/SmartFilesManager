﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include <QDirIterator>
#include <QSet>
#include <QFileInfo>
#include <QTimer>
#include "utils.h"
#include "confighelper.h"
#include "dbhelper.h"
#include "analyser.h"
#include "toolkitinitthread.h"
#include "filetreemodel.h"

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
    void quitWorkingThread();
    void fileUpdaterWait(unsigned long time);

protected:
    void closeEvent(QCloseEvent *event) override;

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
    void showUpdaterDbProgress();
    void notifyResult(int success, int fail);
    void updateFilesList(bool renew = false);

    void fileUpdaterInterrupted();
    void fileUpdaterFinished();
    void analyserInterrupted();

    void on_actionStart_triggered();

    void onStartInitToolkit();
    void onFinishInitToolkit();

    void setupFileTreeView();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    void readyQuit();
    void createTrayIcon();
    void setTrigger();
    void processWorkList(bool triggered = false);

    QString fileSizeHumanReadable(qint64 num);

    ConfigHelper *configHelper;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    SettingsDialog *settingsDialog;
    DBHelper *dbHelper;
    QTimer *triggerTimer;
    Analyser *analyser;

    FileTreeModel *fileTreeModel = nullptr;
};

#endif // MAINWINDOW_H
