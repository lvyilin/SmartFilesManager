#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "utils.h"


class DBHelper : public QObject {
    Q_OBJECT
public:
    explicit DBHelper(const QString &conName, const QString &dbName, QObject *parent = nullptr);

    /**

     * @brief hasIndex 是否文件表是否有文件条目
     * @return 真值
     */
    bool hasIndex() const;

    /**
     * @brief addFiles 添加多文件
     * @param pathSet 文件路径集合
     */
    void addFiles(const QList<File> &filesList);

    /**
     * @brief cleanFiles 清空文件表
     */
    void cleanFiles();

    /**
     * @brief close 关闭数据库
     */
    void close();

    /**
     * @brief getWorkList 获得工作列表
     * @param num 工作列表文件数目，默认500
     * @return 返回一个包含num个文件的列表
     */
    QList<File> &getWorkList(const QString &format, int num = 50);//deprecated
    QList<File> &getWorkList(int num);

    /**
     * @brief initlabels 初始化labels表格
     */
    void initLabels();

    /**
     * @brief setFinish 将文件file的is_finished设为finish
     * @param file 文件
     * @param finish is_finished真值
     */
    void setFinish(const File &file, bool finish);

    /**
     * @brief setValid 将文件file的is_valid值设为valid
     * @param file 文件
     * @param valid is_valid真值
     */
    void setValid(const File &file, bool valid);

    /**
     * @brief setFileProduct 设置文件提取文本、分词后的结果
     * @param fp
     */
    void setFileProduct(FileProduct fp);

signals:
public slots:
private:
    void createTable();
    bool addFile(const File &file);
    QSqlDatabase db;
    QSqlQuery *query;
    QList<File> unfinishedFile ;
};

#endif // DBHELPER_H
