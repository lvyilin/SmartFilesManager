#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QStandardItemModel>
class QSettings;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QSettings *settings, QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;
    QSettings *settingsPtr;
    QStandardItemModel *pathModel;

    bool isUniquePath(const QString &path);

private slots:
    void onAccepted();
    void onChanged();
    void checkRemoveButtonStatus(const QModelIndex &index);
    void on_pushButtonAddPath_clicked();
    void on_pushButtonRemovePath_clicked();
};


#endif // SETTINGSDIALOG_H
