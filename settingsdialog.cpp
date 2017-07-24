#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <qDebug>
#include <QMessageBox>
#include <QStandardPaths>
#include <QItemSelectionModel>

SettingsDialog::SettingsDialog(QSettings *st, QWidget *parent) :
    settingsPtr(st),
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));

    pathModel = new QStandardItemModel();
    ui->pathView->setModel(pathModel);

    ui->pushButtonRemovePath->setEnabled(false);
    connect(ui->pathView,SIGNAL(clicked(QModelIndex)),this,SLOT(checkRemoveButtonStatus(QModelIndex)));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onChanged()
{

}

void SettingsDialog::onAccepted()
{

}

void SettingsDialog::checkRemoveButtonStatus(const QModelIndex &index)
{
    const bool valid = index.isValid();
    ui->pushButtonRemovePath->setEnabled(valid);
}

void SettingsDialog::on_pushButtonAddPath_clicked()
{
    const QString directoryName =
            QFileDialog::getExistingDirectory(this,
                                              tr("选择监控路径"),
                                              QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    if (!directoryName.isEmpty())
    {
        if(isUniquePath(directoryName))
            pathModel->setItem(pathModel->rowCount(),
                               new QStandardItem(QString(directoryName)));
        else
            QMessageBox::information(this,tr("提示"),tr("路径已存在，请重新输入"));
    }
}

bool SettingsDialog::isUniquePath(const QString &path)
{
    for(int i=0;i<pathModel->rowCount();i++)
    {
        if(path==pathModel->item(i)->text())
            return false;
    }
    return true;
}

void SettingsDialog::on_pushButtonRemovePath_clicked()
{
    pathModel->removeRow(ui->pathView->currentIndex().row());
    checkRemoveButtonStatus(ui->pathView->currentIndex());
}
