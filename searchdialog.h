#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include "utils.h"
#include "dbhelper.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(DBHelper *db, QWidget *parent = 0);
    ~SearchDialog();

private slots:
    void on_checkBoxFormat_toggled(bool checked);
    void addFormatFilter();
    void removeFormatFilter();

    void on_checkBoxField_toggled(bool checked);
    void fieldChanged(QString text);

    void on_checkBoxSize_toggled(bool checked);

    void on_spinBoxSizeFrom_valueChanged(int arg1);

    void on_checkBoxDate_toggled(bool checked);

    void on_pushButtonCancel_clicked();

    void on_pushButtonSearch_clicked();

private:
    Ui::SearchDialog *ui;
    DBHelper *dbHelper;
    QPushButton *buttonAdd;
    QPushButton *buttonClear;
    QList<QLabel *> formatLabelList;
    QList<Label> fieldLabels;
    QList<FileResult> fileResults;
    QComboBox *subFieldBox;

    void init();
};

#endif // SEARCHDIALOG_H
