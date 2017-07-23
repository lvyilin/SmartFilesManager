#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QList>
#include <QSettings>
class SettingsDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
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

    void on_actionExit_triggered();

    void on_actionSettings_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    void init();
    void readSettings();
    void writeSettings();

    void reallyQuit();

    void createTrayIcon();

    //设置触发器
    void setTaskTimer();

    //文件索引
    bool hasIndex();
    void buildIndex();
    void updateIndex();

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    SettingsDialog *settingsDialog;
    QSettings settings;

};

#endif // MAINWINDOW_H
