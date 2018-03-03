#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QLineEdit>
#include "searchpopup.h"
#include "utils.h"

class SearchBox : public QLineEdit {
    Q_OBJECT
public:
    explicit SearchBox(const QList<File> *li, QWidget *parent = nullptr);

signals:
    void findFile(const QString path, bool byPath = true);
    void fileNotFound();

public slots:
    void doSearch();
    void doSearchFromSuggest();

private:
    SearchPopup *popup;
    const QList<File> *fileList;
    QString lastText;
    int findNextCount = 0;
};

#endif // SEARCHBOX_H
