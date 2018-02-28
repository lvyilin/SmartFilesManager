#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QPushButton>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("高级搜索"));
    //    QPushButton buttonAdd(QIcon(":/images/icons/add.png"),QString(),this);
    //    QPushButton buttonClear
    //    ui->horizontalLayoutFormat->addWidget();
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_checkBoxFormat_toggled(bool checked)
{
    ui->lineEditFormatAdd->setEnabled(checked);
}
