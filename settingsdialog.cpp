#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <qDebug>

SettingsDialog::SettingsDialog(QSettings *st, QWidget *parent) :
    settingsPtr(st),
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设置"));
    qDebug()<<"aa";
    /*connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onAccepted);
    connect(ui->toolbarStyleComboBox, &QComboBox::currentTextChanged, this, &SettingsDialog::onChanged);
    connect(ui->hideCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::onChanged);
    connect(ui->startAtLoginCheckbox, &QCheckBox::stateChanged, this, &SettingsDialog::onChanged);
    connect(ui->oneInstanceCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::onChanged);
    connect(ui->nativeMenuBarCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::onChanged);*/
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
