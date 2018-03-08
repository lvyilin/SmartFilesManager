#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题
#include "searchdialog.h"
#include "ui_searchdialog.h"
#include <QPushButton>
#include <QIcon>
#include <QMessageBox>


SearchDialog::SearchDialog(DBHelper *db, QWidget *parent) :
    dbHelper(db),
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    setWindowTitle("高级搜索");
    dbHelper->getAllFieldLabels(fieldLabels);
    dbHelper->getFileResults(fileResults, false);
    subFieldBox = new QComboBox(this);
    subFieldBox->setVisible(false);
    ui->horizontalLayoutField->insertWidget(2, subFieldBox, 1);

    ui->lineEditFormatAdd->setMaxLength(8);
    buttonAdd = new QPushButton(QIcon(":/images/icons/add.png"), QString(), this);
    buttonClear = new QPushButton(QIcon(":/images/icons/trashbin.png"), QString(), this);
    connect(buttonAdd, &QPushButton::clicked, this, &SearchDialog::addFormatFilter);
    connect(buttonClear, &QPushButton::clicked, this, &SearchDialog::removeFormatFilter);
    connect(ui->lineEditFormatAdd, &QLineEdit::returnPressed, this, &SearchDialog::addFormatFilter);

    ui->horizontalLayoutFormat->insertWidget(ui->horizontalLayoutFormat->count() - 1, buttonAdd);
    ui->horizontalLayoutFormat->insertWidget(ui->horizontalLayoutFormat->count() - 1, buttonClear);


    for (int i = 0; i < fieldLabels.count(); ++i)
    {
        if (fieldLabels[i].level == 1)
            ui->comboBoxField->addItem(fieldLabels[i].name);
    }
    connect(ui->comboBoxField, SIGNAL(activated(QString)), this, SLOT(fieldChanged(QString)));

    init();
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_checkBoxFormat_toggled(bool checked)
{
    ui->lineEditFormatAdd->setEnabled(checked);
    buttonAdd->setEnabled(checked);
    buttonClear->setEnabled(checked);
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
    formatLabelList << fmtLabel;
    fmtLabel->setEnabled(false);
    ui->horizontalLayoutFormat->insertWidget(ui->horizontalLayoutFormat->count() - 4, fmtLabel);
    ui->lineEditFormatAdd->clear();
}

void SearchDialog::removeFormatFilter()
{
    int i = formatLabelList.count() - 1;
    for (; i >= 0; --i)
    {
        ui->horizontalLayoutFormat->removeWidget(formatLabelList[i]);
        delete formatLabelList[i];
    }
    formatLabelList.clear();
    ui->labelFormatNone->setVisible(true);
}


void SearchDialog::on_checkBoxField_toggled(bool checked)
{
    ui->comboBoxField->setEnabled(checked);
    subFieldBox->setEnabled(checked);
}

void SearchDialog::fieldChanged(QString text)
{
    subFieldBox->clear();
    subFieldBox->addItem("请选择");
    if (text == "请选择")
    {
        subFieldBox->setVisible(false);
    }
    else
    {
        for (Label &l : fieldLabels)
        {
            if (l.parentName == text)
            {
                subFieldBox->addItem(l.name);
            }
        }
        subFieldBox->setVisible(true);
    }
}

void SearchDialog::on_checkBoxSize_toggled(bool checked)
{
    ui->spinBoxSizeFrom->setEnabled(checked);
    ui->spinBoxSizeTo->setEnabled(checked);
    ui->labelSize->setEnabled(checked);
    ui->comboBoxSizeUnit->setEnabled(checked);
}

void SearchDialog::on_spinBoxSizeFrom_valueChanged(int arg1)
{
    ui->spinBoxSizeTo->setMinimum(arg1);
}

void SearchDialog::on_checkBoxDate_toggled(bool checked)
{
    ui->dateEditFrom->setEnabled(checked);
    ui->dateEditTo->setEnabled(checked);
    ui->labelDate->setEnabled(checked);
}

void SearchDialog::on_pushButtonCancel_clicked()
{
    init();
    this->hide();
}

void SearchDialog::on_pushButtonSearch_clicked()
{
    ui->listWidgetResult->clear();
    if (ui->checkBoxSize->isChecked() && ui->spinBoxSizeFrom->value() > ui->spinBoxSizeTo->value())
        QMessageBox::warning(this, "条件错误", "筛选大小条件错误");

    if (ui->checkBoxDate->isChecked() && ui->dateEditFrom->date() > ui->dateEditTo->date())
        QMessageBox::warning(this, "条件错误", "筛选日期条件错误");

    QList<FileResult> result;
    if (!ui->lineEditFilename->text().isEmpty())
    {
        QStringList keywords = ui->lineEditFilename->text().split(' ', QString::SkipEmptyParts);
        for (auto fr : fileResults)
        {
            for (QString kw : keywords)
            {
                if (fr.file.name.contains(kw, Qt::CaseInsensitive))
                {
                    result << fr;
                    break;
                }
            }
        }
    }
    else
    {
        result = fileResults;
    }

    if (!ui->lineEditKeyword->text().isEmpty())
    {
        QStringList keywords = ui->lineEditKeyword->text().split(' ', QString::SkipEmptyParts);
        int i = result.count() - 1;
        for (; i >= 0; --i)
        {
            bool del = true;
            for (QString kw : keywords)
            {
                if (result[i].keywords.contains(kw))
                {
                    del = false;
                    break;
                }
            }
            if (del)
                result.removeAt(i);
        }
    }

    if (ui->checkBoxFormat->isChecked() && formatLabelList.count() != 0)
    {
        int i = result.count() - 1;
        for (; i >= 0; --i)
        {
            bool del = true;
            for (int j = 0; j < formatLabelList.count(); ++j)
            {
                if (result[i].file.format == formatLabelList[j]->text())
                {
                    del = false;
                    break;
                }
            }
            if (del)
            {
                result.removeAt(i);
            }
        }
    }

    if (ui->checkBoxField->isChecked() && ui->comboBoxField->currentText() != "请选择")
    {
        QString field = ui->comboBoxField->currentText();
        if (subFieldBox->currentText() != "请选择")
            field = subFieldBox->currentText();
        int i = result.count() - 1;
        for (; i >= 0; --i)
        {
            bool del = true;
            for (auto lb : result[i].labels)
            {
                if (lb.name == field)
                {
                    del = false;
                    break;
                }
            }
            if (del)
            {
                result.removeAt(i);
            }
        }
    }

    if (ui->checkBoxSize->isChecked())
    {
        int base = ui->comboBoxSizeUnit->currentIndex();
        qint64 min = ui->spinBoxSizeFrom->value();
        qint64 max = ui->spinBoxSizeTo->value();
        for (int i = 0; i < base; ++i)
        {
            min *= 1024;
            max *= 1024;
        }
        int i = result.count() - 1;
        for (; i >= 0; --i)
        {
            if (!(result[i].file.size >= min && result[i].file.size <= max))
                result.removeAt(i);
        }
    }

    if (ui->checkBoxDate->isChecked())
    {
        QDate minDate = ui->dateEditFrom->date();
        QDate maxDate = ui->dateEditTo->date();
        int i = result.count() - 1;
        for (; i >= 0; --i)
        {
            if (!(result[i].file.createTime >= QDateTime(minDate) && result[i].file.createTime <= QDateTime(maxDate)))
                result.removeAt(i);
        }
    }

    for (auto fr : result)
    {
        new QListWidgetItem(fr.file.name, ui->listWidgetResult);
    }
    ui->labelResultNum->setText(QString("共%1个结果").arg(result.count()));
}

void SearchDialog::init()
{
    ui->checkBoxFormat->setChecked(false);
    ui->lineEditFormatAdd->setEnabled(false);
    buttonAdd->setEnabled(false);
    buttonClear->setEnabled(false);

    ui->checkBoxField->setChecked(false);
    ui->comboBoxField->setEnabled(false);

    ui->checkBoxSize->setChecked(false);
    ui->spinBoxSizeFrom->setEnabled(false);
    ui->spinBoxSizeTo->setEnabled(false);
    ui->spinBoxSizeTo->setValue(20);
    ui->labelSize->setEnabled(false);
    ui->comboBoxSizeUnit->setEnabled(false);
    ui->comboBoxSizeUnit->setCurrentIndex(1);

    ui->checkBoxDate->setChecked(false);
    ui->dateEditFrom->setEnabled(false);
    ui->dateEditTo->setEnabled(false);
    ui->labelDate->setEnabled(false);
    ui->dateEditTo->setMaximumDate(QDate::currentDate());
    ui->dateEditFrom->setDate(QDate(2018, 1, 1));
    ui->dateEditTo->setDate(QDate::currentDate());

    ui->listWidgetResult->clear();
    ui->labelResultNum->setText(QString("共0个结果"));
}
