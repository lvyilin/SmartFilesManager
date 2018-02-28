#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QLabel>
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
    void addFormatFilter();
    void removeFormatFilter();

private:
    Ui::SearchDialog *ui;
    QPushButton *buttonAdd;
    QPushButton *buttonClear;
    QList<QLabel *> labelList;
};

#endif // SEARCHDIALOG_H
