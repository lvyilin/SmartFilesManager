﻿#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "utils.h"
#include <QMutex>


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
    void cleanRelations();

    /**
     * @brief close 关闭数据库
     */
    void close();

    /**
     * @brief getWorkList
     * @param li
     * @param maxNum
     */
    void getWorkList(QVector<File> &li, int maxNum);

    /**
     * @brief setFinish 将文件file的is_finished设为finish
     * @param file 文件
     * @param finish is_finished真值
     */
    void setFinished(const File &file, bool finish);

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
    void setFileProduct(const FileProduct &fp);

    void setFileLabels(const FileProduct &fp, const QStringList &labels);

    void getAllFiles(QList<File> &list, QList<int> &idList, bool finished = false);

    /**
     * @brief getFileResultByPath 通过路径path获得FileResult
     * @param path
     * @param fr
     */
    void getFileResultByPath(const QString &path, FileResult &fr);

    /**
     * @brief getFinishedFileResults 获得所有已完成文件的FileResult
     * @param frs 空的FileResult List的引用, 用于储存返回结果
     */

    void getFileResults(QList<FileResult> &frs, bool finished = true);

    void saveFileResults(QList<FileResult> &frs);
    void saveSingleFileResult(const FileResult &fr);
    void getAllFieldLabels(QList<Label> &li);
    QVector<QVector<Label> > getFieldLabels(const QList<File> &li);

    void getFilesHaveLabel(QList<File> &list, const QString &label);
    void getFileResultsHaveLabel(QList<FileResult> &list, const QString &label);
    bool addLabel(const QString &name, const QString &parentName);
    bool removeLabel(const QString &name);

signals:
    void calRelationProgress(int num, int total);
    void dbInterrupted();
    void finishSaveFileResult();
public slots:
    void abortProgress();
private:
    void createTable();
    int getFileId(const QString &path);
    void getFileAndIdByPath(const QString &path, File &file, int &id);
    void getFileResultById(FileResult &fr, int fileId);
    void getFileById(File &f, int fileId);
    int getLabelId(const QString &label, int type = 1);//1:field only; 2:kw only 3:all
    QSqlDatabase db;
    QSqlQuery *query;
    QMutex mutex;
    bool abortFlag = false;
    bool working = false;
};

#endif // DBHELPER_H
