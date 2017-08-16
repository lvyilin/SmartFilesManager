#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QStack>
#include <QModelIndex>
#include <confighelper.h>

namespace Ui {
class SettingsDialog;
}

struct UndoAction
{
    int type;//0 = add 1 = remove
    QString text;
    QStandardItem *item;
    int rowIdx;
};

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(ConfigHelper *ch, QWidget *parent = 0);
    ~SettingsDialog();

signals:
    void pathChanged();

private:
    Ui::SettingsDialog *ui;
    ConfigHelper *configHelper;
    QStack<UndoAction> undoStack;
    bool isUniquePath(const QString &path) const;

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
