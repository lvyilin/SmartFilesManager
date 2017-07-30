#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtSql>
#include <QtCore/QCoreApplication>

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


    QSet<QString> getFiles();

signals:

public slots:
private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // DBHELPER_H
