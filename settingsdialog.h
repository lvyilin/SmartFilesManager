#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QStandardItemModel>

#include <confighelper.h>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
        Q_OBJECT

    public:
        explicit SettingsDialog(ConfigHelper *ch, QWidget *parent = 0);
        ~SettingsDialog();

    private:
        Ui::SettingsDialog *ui;
        ConfigHelper *configHelper;

        bool isUniquePath(const QString &path);

    private slots:
        void onAccepted();
        void onChanged();
        void onCancel();
        void checkRemoveButtonStatus(const QModelIndex &index);
        void on_pushButtonAddPath_clicked();
        void on_pushButtonRemovePath_clicked();
        void radioBtnCpuToggle();
        void radioBtnTimeToggle();
        void on_pushButtonScan_clicked();
};


#endif // SETTINGSDIALOG_H
