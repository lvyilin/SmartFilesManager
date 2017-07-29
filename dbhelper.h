#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

class DBHelper : public QObject {
    Q_OBJECT
public:
    explicit DBHelper(QString &conName, QString &dbName, QObject *parent = nullptr);

    /**
     * @brief hasIndex 是否建立过文件索引
     * @return 真值
     */
    bool hasIndex();

    /**
     * @brief addFile 添加单文件
     * @param path 文件路径
     */
    void addFile(QString &path);

    /**
     * @brief addFiles 添加多文件
     * @param pathSet 文件路径集合
     */
    void addFiles(QSet<QString> &pathSet);

signals:

public slots:
private:
    QSqlDatabase db;
};

#endif // DBHELPER_H
