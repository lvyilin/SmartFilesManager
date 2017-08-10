#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include <QDirIterator>
#include <QSet>
#include <QFileSystemWatcher>
#include <QFileSystemModel>
#include <QFileInfo>
#include "confighelper.h"
#include "dbhelper.h"

class SettingsDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void openSettings();
    void about();
    void rebuildFilesList();

    void updateIndex(QString updateFile);

    void on_actionExit_triggered();

    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    void reallyQuit();
    void createTrayIcon();
    void setTaskTimer();//TODO
    void setFilesMonitor();
    void buildFilesList(bool renew = false);

    ConfigHelper *configHelper;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    SettingsDialog *settingsDialog;
    DBHelper *dbHelper;
    QList<File> filesList;
    //    QFileSystemWatcher *watcher;
    QFileSystemModel *fileModel;

};

#endif // MAINWINDOW_H
