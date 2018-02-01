#ifndef FILETREEMODEL_H
#define FILETREEMODEL_H

#include <QAbstractItemModel>
#include "utils.h"
#include "fileitem.h"

class FileTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit FileTreeModel(const QList<File> fileList, QObject *parent = nullptr);
    ~FileTreeModel();

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    void setupModelData(const QList<File> &fileList);
    FileItem *getTypeItem(const QString &format, const QVector<FileItem *> &typeItems);
    FileItem *rootItem;
};

#endif // FILETREEMODEL_H
