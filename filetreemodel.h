#ifndef FILETREEMODEL_H
#define FILETREEMODEL_H

#include <QAbstractItemModel>
#include "utils.h"
#include "fileitem.h"
#include "dbhelper.h"

class FileTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit FileTreeModel(const QList<File> &li, DBHelper *db, QObject *parent = nullptr);
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

    void setupTypeModelData();
    void setupFieldModelData();
private:
    FileItem *getTypeItem(const QString &format, const QVector<FileItem *> &typeItems);
    FileItem *getParentFieldItem(const QString &parentName, const QVector<FileItem *> &fieldItems);
    const QList<File> &fileList;
    DBHelper *dbHelper;
    FileItem *rootItem = nullptr;
    FileItem *undefinedField = nullptr;
};

#endif // FILETREEMODEL_H
