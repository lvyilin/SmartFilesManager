#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QStandardItemModel>
#include <QDirIterator>
#include <QSet>
#include <QFileInfo>
#include <QTimer>
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

signals:
    /**
     * @brief onFinishedWorkList 用于提醒上次是否成功运行
     */
    void onFinishedWorkList();
protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void openSettings();
    void about();
    void rebuildFilesList();
    void processWorkList();

    void on_actionExit_triggered();

    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    void reallyQuit();
    void createTrayIcon();
    void setTrigger();
    void updateFilesList(bool renew = false);

    ConfigHelper *configHelper;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    SettingsDialog *settingsDialog;
    DBHelper *dbHelper;
    QList<File> filesList;
    QList<File> workList;
    QTimer *triggerTimer;

};

#endif // MAINWINDOW_H
