#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "filetreemodel.h"
#include <QDebug>

FileTreeModel::FileTreeModel(const QList<File> fileList, QObject *parent)
    : QAbstractItemModel(parent)
{
    setupModelData(fileList);
}

FileTreeModel::~FileTreeModel()
{
    delete rootItem;
}

QModelIndex FileTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FileItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FileItem *>(parent.internalPointer());

    FileItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FileTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FileItem *childItem = static_cast<FileItem *>(index.internalPointer());
    FileItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FileTreeModel::rowCount(const QModelIndex &parent) const
{
    FileItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FileItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int FileTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<FileItem *>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant FileTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FileItem *item = static_cast<FileItem *>(index.internalPointer());

    if (role == Qt::ToolTipRole)
        return item->data(1);

    if (role != Qt::DisplayRole)
        return QVariant();


    return item->data(index.column());
}

Qt::ItemFlags FileTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
QVariant FileTreeModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

void FileTreeModel::setupModelData(const QList<File> &fileList)
{
    rootItem = new FileItem("文件", nullptr);
    QVector<FileItem *> typeItems;
    typeItems << new FileItem("文档", rootItem);
    typeItems << new FileItem("图片", rootItem);
    typeItems << new FileItem("音频", rootItem);
    typeItems << new FileItem("视频", rootItem);
    typeItems << new FileItem("其它", rootItem);

    QVector<FileItem *> formatItems;
    QVector<QString> formatList;
    for (File f : fileList)
    {
        FileItem *tpItem = getTypeItem(f.format, typeItems);
        FileItem *fmtItem;
        if (formatList.contains(f.format))
        {
            int idx = formatList.indexOf(f.format);
            fmtItem = formatItems.at(idx);
        }
        else
        {
            fmtItem = new FileItem(f.format, tpItem);
            formatItems << fmtItem;
            formatList << f.format;
        }
        new FileItem(f, fmtItem);
    }
}

FileItem *FileTreeModel::getTypeItem(const QString &format, const QVector<FileItem *> &typeItems)
{
    QString tp;
    if (DOCUMENT_FORMATS.contains(format, Qt::CaseInsensitive)) tp = "文档";
    else if (IMAGE_FORMATS.contains(format, Qt::CaseInsensitive))tp = "图片";
    else if (AUDIO_FORMATS.contains(format, Qt::CaseInsensitive))tp = "音频";
    else if (VIDEO_FORMATS.contains(format, Qt::CaseInsensitive))tp = "视频";
    else tp = "其它";

    for (FileItem *item : typeItems)
    {
        if (item->itemName == tp)
            return item;
    }
    return nullptr;
}
