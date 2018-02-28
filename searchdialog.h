#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

private slots:

    void on_checkBoxFormat_toggled(bool checked);

private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
