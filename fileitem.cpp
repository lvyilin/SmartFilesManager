#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "fileitem.h"

FileItem::FileItem(const QString &s, FileItem *parent)
{
    itemName = s;
    parentItem = parent;
    if (parent != nullptr)
        parentItem->appendChildren(this);
    itemData << s;
}


FileItem::FileItem(const File &file, FileItem *parent)
{
    itemData << file.name;
//             << file.format
//             << file.path
//             << file.size
//             << file.createTime
//             << file.modifyTime;
    parentItem = parent;
    parentItem->appendChildren(this);

}

FileItem::~FileItem()
{
    qDeleteAll(childItems);
}

void FileItem::appendChildren(FileItem *child)
{
    childItems.append(child);
}

FileItem *FileItem::child(int row)
{
    return childItems.value(row);
}

int FileItem::childCount() const
{
    return childItems.count();
}

FileItem *FileItem::parent()
{
    return parentItem;
}

int FileItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<FileItem *>(this));
    return 0;
}

int FileItem::columnCount() const
{
    return itemData.count();
}

QVariant FileItem::data(int column) const
{
    return itemData.value(column);
}
