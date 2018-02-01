#ifndef FILEITEM_H
#define FILEITEM_H
#include "utils.h"
#include <QVariant>

class FileItem {
public:
    FileItem(const QString &s, FileItem *parent);
    FileItem(const File &file, FileItem *parent);
    ~FileItem();

    void appendChildren(FileItem *child);
    FileItem *child(int row);
    int childCount() const;
    FileItem *parent();
    int row() const;
    int columnCount() const;
    QVariant data(int column) const;

    QString itemName;
private:
    QList<QVariant> itemData;

    FileItem *parentItem;
    QList<FileItem *> childItems;

};

#endif // FILEITEM_H
