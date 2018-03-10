#ifndef SEARCHPOPUP_H
#define SEARCHPOPUP_H

#include <QObject>
#include <QLineEdit>
#include <QTreeWidget>
#include <QHeaderView>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include "utils.h"

class SearchPopup : public QObject {
    Q_OBJECT
public:
    explicit SearchPopup(const QList<File> *li, QLineEdit *parent = nullptr);
    ~SearchPopup();
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void showSearchResult(const QList<File> &choices);

signals:
    void done();
public slots:

    void doneSearch();
    void preventSuggest();
    void autoSuggest();

private:
    QLineEdit *editor;
    QTreeWidget *popup;
    QTimer *timer;
    const QList<File> *fileList;
};

#endif // SEARCHPOPUP_H
