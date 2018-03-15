#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题
#ifndef LABELDIALOG_H
#define LABELDIALOG_H

#include <QDialog>
#include "dbhelper.h"
#include "utils.h"
namespace Ui {
class LabelDialog;
}

class LabelDialog : public QDialog {
    Q_OBJECT

public:
    explicit LabelDialog(DBHelper *db, QWidget *parent = 0);
    ~LabelDialog();
    void setLineText(QString text);

private slots:
    void fieldChanged(QString text);
    void on_buttonBox_accepted();

    void on_comboBoxOp_currentIndexChanged(int index);

private:
    Ui::LabelDialog *ui;
    DBHelper *dbHelper;
    QList<Label> fieldLabels;
};

#endif // LABELDIALOG_H
