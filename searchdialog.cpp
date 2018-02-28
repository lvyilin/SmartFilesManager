#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QPushButton>
#include <QIcon>


SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("高级搜索"));
    buttonAdd = new QPushButton(QIcon(":/images/icons/add.png"), QString(), this);
    buttonClear = new QPushButton(QIcon(":/images/icons/trashbin.png"), QString(), this);
    connect(buttonAdd, &QPushButton::clicked, this, &SearchDialog::addFormatFilter);
    connect(buttonClear, &QPushButton::clicked, this, &SearchDialog::removeFormatFilter);
    connect(ui->lineEditFormatAdd, &QLineEdit::returnPressed, this, &SearchDialog::addFormatFilter);
    ui->horizontalLayoutFormat->insertWidget(ui->horizontalLayoutFormat->count() - 1, buttonAdd);
    ui->horizontalLayoutFormat->insertWidget(ui->horizontalLayoutFormat->count() - 1, buttonClear);
    ui->checkBoxFormat->setChecked(false);
    ui->checkBoxField->setChecked(false);
    ui->lineEditFormatAdd->setEnabled(false);
    ui->buttonBox->setEnabled(false);
    ui->buttonBox->setEnabled(false);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_checkBoxFormat_toggled(bool checked)
{
    ui->lineEditFormatAdd->setEnabled(checked);
    ui->buttonBox->setEnabled(checked);
    ui->buttonBox->setEnabled(checked);
}

void SearchDialog::addFormatFilter()
{
    if (ui->lineEditFormatAdd->text().isEmpty())
        return;
    if (ui->labelFormatNone->isVisible())
    {
        ui->labelFormatNone->setVisible(false);
    }
    QLabel *fmtLabel = new QLabel(ui->lineEditFormatAdd->text(), this);
    labelList << fmtLabel;
    fmtLabel->setEnabled(false);
    ui->horizontalLayoutFormat->insertWidget(ui->horizontalLayoutFormat->count() - 4, fmtLabel);
    ui->lineEditFormatAdd->clear();
}

void SearchDialog::removeFormatFilter()
{
    int i = labelList.count() - 1;
    for (; i >= 0; --i)
    {
        ui->horizontalLayoutFormat->removeWidget(labelList[i]);
        delete labelList[i];
    }
    labelList.clear();
    ui->labelFormatNone->setVisible(true);
}

