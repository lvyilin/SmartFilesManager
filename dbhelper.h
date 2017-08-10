#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
struct File
{
    QString name;
    QString format;
    QString path;
    qint64 size;
    QDateTime createTime;
    QDateTime modifyTime;
    bool isFinished;
};

class DBHelper : public QObject {
    Q_OBJECT
public:
    explicit DBHelper(QString &conName, QString &dbName, QObject *parent = nullptr);

    /**
     * @brief hasIndex 是否文件表是否有文件条目
     * @return 真值
     */
    bool hasIndex();

    /**
     * @brief addFiles 添加多文件
     * @param pathSet 文件路径集合
     */
    void addFiles(QList<File> &filesList);

    /**
     * @brief cleanFiles 清空文件表
     */
    void cleanFiles();

    /**
     * @brief close 关闭数据库
     */
    void close();

    /**
     * @brief getFiles 获得文件（先不写，可能废除）
     * @return 文件表所有条目
     */
    QList<File> getFiles();

    /**
     * @brief getWorkList 获得工作列表
     * @param num 工作列表文件数目，默认500
     * @return 返回一个包含num个文件的列表
     */
    QList<File> getWorkList(int num = 500);

    /**
     * @brief setFinished 将指定文件is_finished设为已完成
     * @param file 指定文件
     */
    void setFinished(File file);

signals:

public slots:
private:
    void createTable();
    void addFile(File &file);

    QSqlDatabase db;
    QSqlQuery *query;
};

#endif // DBHELPER_H
