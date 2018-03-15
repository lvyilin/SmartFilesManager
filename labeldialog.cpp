#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "labeldialog.h"
#include "ui_labeldialog.h"
#include <QMessageBox>
LabelDialog::LabelDialog(DBHelper *db, QWidget *parent) :
    dbHelper(db),
    QDialog(parent),
    ui(new Ui::LabelDialog)
{
    ui->setupUi(this);
    dbHelper->getAllFieldLabels(fieldLabels);
    for (int i = 0; i < fieldLabels.count(); ++i)
    {
        if (fieldLabels[i].level == 1)
            ui->comboBoxField->addItem(fieldLabels[i].name);
    }
    ui->comboBoxSubField->setVisible(false);
    connect(ui->comboBoxField, SIGNAL(activated(QString)), this, SLOT(fieldChanged(QString)));
}

LabelDialog::~LabelDialog()
{
    delete ui;
}

void LabelDialog::setLineText(QString text)
{
    ui->lineEdit->setText(text);
}

void LabelDialog::fieldChanged(QString text)
{
    ui->comboBoxSubField->clear();
    ui->comboBoxSubField->addItem("请选择");
    if (text == "请选择")
    {
        ui->comboBoxSubField->setVisible(false);
    }
    else
    {
        for (Label &l : fieldLabels)
        {
            if (l.parentName == text)
            {
                ui->comboBoxSubField->addItem(l.name);
            }
        }
        ui->comboBoxSubField->setVisible(true);
    }
}

void LabelDialog::on_buttonBox_accepted()
{
    if (ui->comboBoxField->currentIndex() == 0 || ui->comboBoxSubField->currentIndex() == 0)
    {
        QMessageBox::warning(this,
                             "添加失败",
                             "请选择一个领域!");
        return;
    }
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,
                             "添加失败",
                             "标签名为空!");
        return;
    }
    bool result;
    if (ui->comboBoxOp->currentIndex() == 0)
    {
        result = dbHelper->addLabel(ui->lineEdit->text(), ui->comboBoxSubField->currentText());
    }
    else
    {
        result = dbHelper->removeLabel(ui->lineEdit->text());
    }
    if (result)
    {
        QString op = ui->comboBoxOp->currentIndex() == 0 ? "添加" : "删除";
        QMessageBox::information(this,
                                 QString("%1成功").arg(op),
                                 QString("已%1标签: %2").arg(op).arg(ui->lineEdit->text()));
    }
    else
    {
        QMessageBox::warning(this,
                             "添加失败",
                             "标签已存在或(父)标签不合法.");
    }

}

void LabelDialog::on_comboBoxOp_currentIndexChanged(int index)
{
    ui->comboBoxField->setEnabled(index == 0);
    ui->comboBoxSubField->setEnabled(index == 0);
}
