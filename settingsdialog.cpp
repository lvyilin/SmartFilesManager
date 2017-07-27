#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

/*
    TODO
    Model未保存撤销问题
*/


#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <qDebug>
#include <QMessageBox>
#include <QStandardPaths>

SettingsDialog::SettingsDialog(ConfigHelper *ch, QWidget *parent) :
    configHelper(ch),
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));
    ui->pathView->setModel(configHelper->pathModel);
    ui->checkBoxStartAtBoot->setChecked(configHelper->isStartAtBoot());
    ui->pushButtonRemovePath->setEnabled(false);
    ui->spinBoxCpu->setRange(5, 20);
    ui->spinBoxCpu->setValue(configHelper->getCpuTriggerPercent());
    ui->timeEdit->setTime(configHelper->getTimeTriggerPoint());
    ui->spinBoxScanInterval->setValue(configHelper->getScanInterval());

    connect(ui->radioButtonCpu, SIGNAL(clicked(bool)), this, SLOT(radioBtnCpuToggle()));
    connect(ui->radioButtonCpu, SIGNAL(clicked(bool)), this, SLOT(onChanged()));
    connect(ui->radioButtonTime, SIGNAL(clicked(bool)), this, SLOT(radioBtnTimeToggle()));
    connect(ui->radioButtonTime, SIGNAL(clicked(bool)), this, SLOT(onChanged()));
    connect(ui->spinBoxScanInterval, SIGNAL(valueChanged(int)), this, SLOT(onChanged()));
    connect(ui->pathView, SIGNAL(clicked(QModelIndex)), this, SLOT(checkRemoveButtonStatus(QModelIndex)));

    if (configHelper->getRunningStrategy() == ConfigHelper::CpuTrigger)
    {
        ui->radioButtonCpu->setChecked(true);
        emit ui->radioButtonCpu->clicked();
    }
    else
    {
        ui->radioButtonTime->setChecked(true);
        emit ui->radioButtonTime->clicked();
    }
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onCancel()));
    connect(ui->checkBoxStartAtBoot, SIGNAL(stateChanged(int)), this, SLOT(onChanged()));
    connect(ui->spinBoxCpu, SIGNAL(valueChanged(int)), this, SLOT(onChanged()));
    connect(ui->timeEdit, SIGNAL(timeChanged(QTime)), this, SLOT(onChanged()));

    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onChanged()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(true);
}

void SettingsDialog::onAccepted()
{
    if (ui->radioButtonCpu->isChecked())
    {
        configHelper->setSettings(ui->checkBoxStartAtBoot->isChecked(),
                                  ui->spinBoxCpu->value(),
                                  ui->spinBoxScanInterval->value());
    }
    else if (ui->radioButtonTime->isChecked())
    {
        configHelper->setSettings(ui->checkBoxStartAtBoot->isChecked(),
                                  ui->timeEdit->time(),
                                  ui->spinBoxScanInterval->value());
    }

    configHelper->saveSettings();
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    this->accept();
}

void SettingsDialog::onCancel()
{
    emit configHelper->pathModel->revert();
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(false);
    this->reject();
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
        if (isUniquePath(directoryName))
        {
            auto path = new QStandardItem(QString(directoryName));
            path->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            configHelper->pathModel->appendRow(path);
            emit onChanged();
        }
        else
            QMessageBox::information(this, tr("提示"), tr("路径已存在，请重新输入"));
    }
}

bool SettingsDialog::isUniquePath(const QString &path)
{
    for (int i = 0; i < configHelper->pathModel->rowCount(); i++)
    {
        if (path == configHelper->pathModel->item(i)->text())
            return false;
    }
    return true;
}

void SettingsDialog::on_pushButtonRemovePath_clicked()
{
    configHelper->pathModel->removeRow(ui->pathView->currentIndex().row());
    checkRemoveButtonStatus(ui->pathView->currentIndex());
    emit onChanged();
}

void SettingsDialog::radioBtnCpuToggle()
{
    if (ui->radioButtonCpu->isChecked())
    {
        ui->spinBoxCpu->setEnabled(true);
        ui->timeEdit->setEnabled(false);
    }

}

void SettingsDialog::radioBtnTimeToggle()
{
    if (ui->radioButtonTime->isChecked())
    {
        ui->timeEdit->setEnabled(true);
        ui->spinBoxCpu->setEnabled(false);
    }
}

void SettingsDialog::on_pushButtonScan_clicked()
{

}
