﻿#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "dbhelper.h"
#include <qDebug>
#include <QCoreApplication>

DBHelper::DBHelper(const QString &conName, const QString &dbName, QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE", conName); //添加数据库驱动 已制定链接名称
    db.setDatabaseName(dbName); //数据库链接命名
    if (!db.open()) //打开数据库
    {
        qDebug() << "Error: connection with database fail";
        qDebug() << db.lastError().text();
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

    query = new QSqlQuery(db);
    createTable();
}

bool DBHelper::hasIndex() const
{
    query->exec("SELECT name FROM files limit 1");
    if (!query->next())
    {
        return false;
    }
    return true;
}

void DBHelper::addFiles(const QList<File> &filesList)
{
    working = true;
    if (!db.transaction())
    {
        qDebug() << "db transaction unsupported?";
    }
    foreach (File file, filesList)
    {
        query->prepare("insert or replace into files (name, format, path, size, create_time, modify_time, is_finished) "
                       "values(:name, :format, :path, :size, :create_time, :modify_time, :is_finished)");
        query->addBindValue(file.name);
        query->addBindValue(file.format);
        query->addBindValue(file.path);
        query->addBindValue(file.size);
        query->addBindValue(file.createTime.toString(Qt::ISODate));
        query->addBindValue(file.modifyTime.toString(Qt::ISODate));
        query->addBindValue(file.isFinished);
        query->exec();
    }
    if (!db.commit())
    {
        qDebug() << "db commit error: " << db.lastError();
        db.rollback();
    }
    working = false;
}

void DBHelper::cleanFiles()
{
    query->prepare("delete from files");
    if (!query->exec())
        qDebug() << query->lastError();
    else
        qDebug() << "table cleared" ;
}

void DBHelper::cleanRelations()
{
    query->exec("delete from file_relations");
}

void DBHelper::close()
{
    query->finish();
    db.close();
}

void DBHelper::getWorkList(QVector<File> &li, int maxNum)
{
    working = true;
    //仅返回可以处理的格式的文件
    for (QString fmt : SUPPORTED_FORMATS)
    {
        if (!query->exec(QString("select * from files where format = \"%1\" and is_finished = 0 and is_valid = 1 limit %2").arg(fmt).arg(maxNum)))
        {
            qDebug() << "【getWorkList】 error: " << query->lastError().text();
        }
        while (query->next())
        {
            File temp;
            temp.name = query->value(1).toString();
            temp.format = query->value(2).toString();
            temp.path = query->value(3).toString();
            temp.size = query->value(4).toLongLong();
            temp.createTime = query->value(5).toDateTime();
            temp.modifyTime = query->value(6).toDateTime();
            temp.isFinished = query->value(7).toBool();
            li.append(temp);
        }
    }
    working = false;
}

void DBHelper::createTable()
{
    if (!query->exec("create table if not exists labels("
                     "id integer primary key autoincrement NOT NULL,"
                     "name varchar(255) NOT NULL,"
                     "level int, "
                     "parent int,"
                     "is_leaf bool NOT NULL,"
                     "type varchar(20) NOT NULL"
                     ")"))
        qDebug() << "labels create false" << query->lastError().text();
    else
        qDebug() << "table create success";

    if (!query->exec("create table if not exists files("
                     "id integer primary key autoincrement NOT NULL,"
                     "name varchar(255) NOT NULL,"
                     "format varchar(10) NOT NULL,"
                     "path varchar(255) NOT NULL UNIQUE,"
                     "size unsigned big int NOT NULL,"
                     "create_time DATETIME NOT NULL,"
                     "modify_time DATETIME NOT NULL,"
                     "is_finished BOOLEAN NOT NULL,"
                     "is_valid BOOLEAN DEFAULT 1 NOT NULL)"))
        qDebug() << "files create false" << query->lastError().text();
    else
        qDebug() << "table create success";

    if (!query->exec("create table if not exists file_labels(file_id integer,label_id integer,FOREIGN KEY(file_id) REFERENCES files(id) on delete cascade on update cascade ,FOREIGN KEY(label_id) REFERENCES labels(id), constraint pk_t2 primary key (file_id,label_id))"))
        qDebug() << "file_labels create false" << query->lastError().text();
    else
        qDebug() << "table create success";

    if (!query->exec("create table if not exists file_relations(file_a_id int, file_b_id int, keyword_degree double NOT NULL, label_degree double NOT NULL, attribute_degree double NOT NULL, FOREIGN KEY(file_a_id) REFERENCES files(id) on delete cascade on update cascade ,FOREIGN KEY(file_b_id) REFERENCES files(id) on delete cascade on update cascade  ,constraint pk_t2 primary key (file_a_id,file_b_id))"))
        qDebug() << "file_relations create false" << query->lastError().text();
    else
        qDebug() << "table create success";
    if (!query->exec("create table if not exists file_keywords(file_id integer,keyword varchar(255),weight double NOT NULL,FOREIGN KEY(file_id) REFERENCES files(id) on delete cascade on update cascade ,constraint pk_t2 primary key (file_id,keyword))"))
        qDebug() << "file_relations create false" << query->lastError().text();
    else
        qDebug() << "table create success";
    if (!query->exec("PRAGMA foreign_keys = ON"))
        qDebug() << "foreign_keys didn't open" << query->lastError().text();
    else
        qDebug() << "foreign_keys have opened"; //打开外键约束
}

int DBHelper::getFileId(const QString &path)
{
    query->prepare("select id from files where path=:path");
    query->bindValue(":path", path);
    query->exec();
    if (query->next())
    {
        return query->value(0).toInt();
    }

    qDebug() << "cannot find file id" << path;
    return 0;
}

void DBHelper::setFinished(const File &file, bool finish)
{
    mutex.lock();
    query->exec(QString("update files set is_finished = %1 where path = \"%2\"").arg(finish).arg(file.path));
    mutex.unlock();
}

void DBHelper::setValid(const File &file, bool valid)
{
    mutex.lock();
    query->exec(QString("update files set is_valid = %1 where path = \"%2\" ").arg(valid).arg(file.path));
    mutex.unlock();
}

void DBHelper::setFileProduct(const FileProduct &fp)
{
    mutex.lock();

    int fileId = getFileId(fp.file.path);

    QMapIterator<QString, double> map(fp.keywords);
    if (!db.transaction())
    {
        qDebug() << "db transaction unsupported?";
    }
    while (map.hasNext())
    {
        map.next();
        QString tempKeyword = map.key();
        double tempWeight = map.value();
        if (!query->exec(
                    QString("insert into file_keywords(file_id,keyword,weight) values(%1, \"%2\", \"%3\")")
                    .arg(fileId).arg(tempKeyword).arg(tempWeight)))
        {
            qDebug() << "save file product failed: " << query->lastError().text();
        }
    }
    if (!db.commit())
    {
        qDebug() << "db commit error: " << db.lastError();
        db.rollback();
    }

    mutex.unlock();
}

void DBHelper::setFileLabels(const FileProduct &fp, const QStringList &labels)
{
    mutex.lock();
    int fileId = getFileId(fp.file.path);

    QMap<int, QVector<int> > countMap; //计数每个关键词的标签所在领域
    for (int i = 0; i < labels.count(); ++i)
    {
        query->prepare("select id, parent from labels where name=:name and is_leaf=1");
        query->bindValue(":name", labels[i]);
        query->exec();
        while (query->next())
        {
            int id = query->value(0).toInt();
            int parentId = query->value(1).toInt();
            countMap[parentId].append(id);
        }
    }
    QMapIterator<int, QVector<int> > iter(countMap);
    int maxSize = 0;
    QVector<int> maxKeys;
    while (iter.hasNext())
    {
        iter.next();
        if (iter.value().count() > maxSize)
        {
            maxSize = iter.value().count();
            maxKeys.clear();
            maxKeys << iter.key();
        }
        else if (iter.value().count() == maxSize)
            maxKeys << iter.key();
    }
    if (maxSize >= LABEL_JUDGEMENT_NEEDED_KEYWORD_NUMBER)
    {
        for (int key : maxKeys)
        {
            int parentId = key;
            int id = countMap[key].at(0);
            //仅第一个标签需要向上打父标签
            while (true)
            {
                query->prepare("insert into file_labels(file_id ,label_id) values(:file_id, :label_id)");
                query->bindValue(":file_id", fileId);
                query->bindValue(":label_id", id);
                query->exec();
                if (parentId == 0)break;
                query->prepare("select id, parent from labels where id=:id");
                query->bindValue(":id", parentId);
                query->exec();
                if (query->next())
                {
                    id = query->value(0).toInt();
                    parentId = query->value(1).toInt();
                }
                else break;
            }
            for (int i = 1; i < countMap[key].count(); ++i)
            {
                query->prepare("insert into file_labels(file_id ,label_id) values(:file_id, :label_id)");
                query->bindValue(":file_id", fileId);
                query->bindValue(":label_id", countMap[key][i]);
                query->exec();
            }
        }
    }

    mutex.unlock();
}

void DBHelper::getFileAndIdByPath(const QString &path, File &file, int &id)
{
    query->prepare("select * from files where path=:path");
    query->addBindValue(path);
    query->exec();
    if (!query->next())
        return;

    id = query->value(0).toInt();
    file.name = query->value(1).toString();
    file.format = query->value(2).toString();
    file.path = query->value(3).toString();
    file.size = query->value(4).toLongLong();
    file.createTime = query->value(5).toDateTime();
    file.modifyTime = query->value(6).toDateTime();
    file.isFinished = query->value(7).toBool();
    file.isValid = query->value(8).toBool();
}

void DBHelper::getAllFiles(QList<File> &list, QList<int> &idList, bool finished)
{
    if (!finished)
        query->exec("select * from files");
    else
        query->exec("select * from files where is_finished=1");
    list.clear();
    idList.clear();
    while (query->next())
    {
        File temp;
        temp.name = query->value(1).toString();
        temp.format = query->value(2).toString();
        temp.path = query->value(3).toString();
        temp.size = query->value(4).toLongLong();
        temp.createTime = query->value(5).toDateTime();
        temp.modifyTime = query->value(6).toDateTime();
        temp.isFinished = query->value(7).toBool();
        idList << query->value(0).toInt();
        list << temp;
    }
}

void DBHelper::getFileResultByPath(const QString &path, FileResult &fr)
{

    File file;
    int fileId;
    getFileAndIdByPath(path, file, fileId);

    if (!file.isValid)
        return;

    fr.file = file;
    getFileResultById(fr, fileId);
}


void DBHelper::getFileResultById(FileResult &fr, int fileId)
{
    //get keywords
    query->prepare("select * from file_keywords where file_id=:id");
    query->bindValue(":id", fileId);
    query->exec();
    while (query->next())
    {
        fr.keywords.insert(query->value(1).toString(), query->value(2).toDouble());
    }

    //get labels
    query->prepare("select name, level, parent, type, id from labels where id in(select label_id from file_labels where file_id=:id)");
    query->bindValue(":id", fileId);
    query->exec();
    QVector<int> parentIds;
    QVector<int> ids;
    while (query->next())
    {
        Label lb;
        lb.name = query->value(0).toString();
        lb.level = query->value(1).toInt();
        lb.type = query->value(3).toString();
        parentIds <<  query->value(2).toInt();
        ids << query->value(4).toInt();
        fr.labels << lb;
    }
    for (int i = 0; i < parentIds.count(); ++i)
    {
        if (fr.labels[i].type == "field" && parentIds[i] != 0)
        {
            /*query->prepare("select name from labels where id=:id");
            query->addBindValue(parentIds[i]);
            query->exec();
            if (query->next())
            {
                fr.labels[i].parentName = query->value(0).toString();
            }*/
            //reduce a db query
            for (int j = 0; j < ids.count(); ++j)
            {
                if (parentIds[i] == ids[j])
                {
                    fr.labels[i].parentName = fr.labels[j].name;
                    break;
                }
            }
        }
    }

    //get relations
    query->prepare("select * from file_relations where file_a_id=:id");
    query->bindValue(":id", fileId);
    query->exec();
    QList<int> relationIdList;
    while (query->next())
    {
        relationIdList << query->value(1).toInt();
        Relation rl;
        rl.keywordDegree = query->value(2).toDouble();
        rl.labelDegree = query->value(3).toDouble();
        rl.attributeDegree = query->value(4).toDouble();
        fr.relations << rl;
    }
    for (int i = 0; i < fr.relations.size(); ++i)
    {
        getFileById(fr.relations[i].file, relationIdList[i]);
    }

}


void DBHelper::getFileById(File &f, int fileId)
{
    query->prepare("select * from files where id=:id");
    query->bindValue(":id", fileId);
    query->exec();
    if (query->next())
    {
        f.name = query->value(1).toString();
        f.format = query->value(2).toString();
        f.path = query->value(3).toString();
        f.size = query->value(4).toLongLong();
        f.createTime = query->value(5).toDateTime();
        f.modifyTime = query->value(6).toDateTime();
        f.isFinished = query->value(7).toBool();
    }
}

int DBHelper::getLabelId(const QString &label, int type)
{
    if (type == 1)
        query->prepare("select id from labels where name=? and type='field'");
    else if (type == 2)
        query->prepare("select id from labels where name=? and type='keyword'");
    else
        query->prepare("select id from labels where name=?");
    query->addBindValue(label);
    query->exec();
    if (query->next())
        return query->value(0).toInt();
    else return 0;
}

void DBHelper::getFileResults(QList<FileResult> &frs, bool finished)
{
    QList<File> list;
    QList<int> idList;
    getAllFiles(list, idList, finished);
    for (int i = 0; i < list.count(); ++i)
    {
        //        QCoreApplication::processEvents();
        FileResult fr;
        fr.file = list[i];
        getFileResultById(fr, idList[i]);
        frs << fr;
    }
}

void DBHelper::saveFileResults(QList<FileResult> &frs)
{
    working = true;
    const int size = frs.size();
    for (int i = 0; i < size; ++i)
    {
        emit calRelationProgress(i + 1, size);
        int id = getFileId(frs[i].file.path);
        for (int j = 0; j < frs[i].relations.size(); ++j)
        {
            if (abortFlag)
                return;
            QCoreApplication::processEvents();
            //            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            int idB = getFileId(frs[i].relations[j].file.path);
            db.transaction();

            query->prepare("insert or replace into file_relations(file_a_id, file_b_id, keyword_degree, label_degree, attribute_degree) "
                           "values(:id, :id_b, :kw, :lb, :attr)");
            query->bindValue(":id", id);
            query->bindValue(":id_b", idB);
            query->bindValue(":kw", frs[i].relations[j].keywordDegree);
            query->bindValue(":lb", frs[i].relations[j].labelDegree);
            query->bindValue(":attr", frs[i].relations[j].attributeDegree);
            query->exec();

            query->prepare("insert or replace into file_relations(file_a_id, file_b_id, keyword_degree, label_degree, attribute_degree) "
                           "values(:id, :id_b, :kw, :lb, :attr)");
            query->bindValue(":id", idB);
            query->bindValue(":id_b", id);
            query->bindValue(":kw", frs[i].relations[j].keywordDegree);
            query->bindValue(":lb", frs[i].relations[j].labelDegree);
            query->bindValue(":attr", frs[i].relations[j].attributeDegree);
            query->exec();

            if (!db.commit())
            {
                qDebug() << "db commit error: " << db.lastError();
                db.rollback();
            }
        }
    }
    working = false;
    emit finishSaveFileResult();
}

void DBHelper::saveSingleFileResult(const FileResult &fr)
{
    mutex.lock();
    int id = getFileId(fr.file.path);
    for (int j = 0; j < fr.relations.size(); ++j)
    {
        int idB = getFileId(fr.relations[j].file.path);
        db.transaction();

        query->prepare("insert or replace into file_relations(file_a_id, file_b_id, keyword_degree, label_degree, attribute_degree) "
                       "values(:id, :id_b, :kw, :lb, :attr)");
        query->bindValue(":id", id);
        query->bindValue(":id_b", idB);
        query->bindValue(":kw", fr.relations[j].keywordDegree);
        query->bindValue(":lb", fr.relations[j].labelDegree);
        query->bindValue(":attr", fr.relations[j].attributeDegree);
        query->exec();

        query->prepare("insert or replace into file_relations(file_a_id, file_b_id, keyword_degree, label_degree, attribute_degree) "
                       "values(:id, :id_b, :kw, :lb, :attr)");
        query->bindValue(":id", idB);
        query->bindValue(":id_b", id);
        query->bindValue(":kw", fr.relations[j].keywordDegree);
        query->bindValue(":lb", fr.relations[j].labelDegree);
        query->bindValue(":attr", fr.relations[j].attributeDegree);
        query->exec();

        if (!db.commit())
        {
            qDebug() << "db commit error: " << db.lastError();
            db.rollback();
        }
    }
    mutex.unlock();
    emit finishSaveFileResult();
}

void DBHelper::getAllFieldLabels(QList<Label> &li)
{
    query->exec("select name, level, parent,id from labels where type='field'");
    QVector<int> parentIds;
    QVector<int> ids;
    while (query->next())
    {
        Label lb;
        lb.name = query->value(0).toString();
        lb.level = query->value(1).toInt();
        lb.type = "field";
        parentIds <<  query->value(2).toInt();
        ids << query->value(3).toInt();
        li << lb;
    }
    for (int i = 0; i < parentIds.count(); ++i)
    {
        if (parentIds[i] != 0)
        {
            for (int j = 0; j < ids.count(); ++j)
            {
                if (parentIds[i] == ids[j])
                {
                    li[i].parentName = li[j].name;
                    break;
                }
            }
        }
    }
}

QVector<QVector<Label> > DBHelper::getFieldLabels(const QList<File> &li)
{
    QVector<QVector<Label> > lbList(li.count());
    for (int i = 0; i < li.count(); ++i)
    {
        int fileId = getFileId(li[i].path);
        query->prepare("select name, level, parent, type, id from labels"
                       " where id in(select label_id from file_labels where file_id=:id) and type='field'");
        query->bindValue(":id", fileId);
        query->exec();
        QVector<int> parentIds;
        QVector<int> ids;
        while (query->next())
        {
            Label lb;
            lb.name = query->value(0).toString();
            lb.level = query->value(1).toInt();
            lb.type = query->value(3).toString();
            parentIds <<  query->value(2).toInt();
            ids << query->value(4).toInt();
            lbList[i] << lb;
        }

        for (int j = 0; j < parentIds.count(); ++j)
        {
            if (parentIds[j] != 0)
            {
                //reduce a db query
                for (int k = 0; k < ids.count(); ++k)
                {
                    if (parentIds[j] == ids[k])
                    {
                        lbList[i][j].parentName = lbList[i][k].name;
                        break;
                    }
                }
            }
        }
    }
    return lbList;
}

void DBHelper::getFilesHaveLabel(QList<File> &list, const QString &label)
{
    int labelId = getLabelId(label);
    if (labelId == 0)return;

    query->prepare("select * from files where id in (select file_id from file_labels where label_id=?)");
    query->addBindValue(labelId);
    query->exec();
    while (query->next())
    {
        File temp;
        temp.name = query->value(1).toString();
        temp.format = query->value(2).toString();
        temp.path = query->value(3).toString();
        temp.size = query->value(4).toLongLong();
        temp.createTime = query->value(5).toDateTime();
        temp.modifyTime = query->value(6).toDateTime();
        temp.isFinished = query->value(7).toBool();
        list << temp;
    }
}

void DBHelper::getFileResultsHaveLabel(QList<FileResult> &list, const QString &label)
{
    int labelId = getLabelId(label);
    if (labelId == 0)return;

    query->prepare("select file_id from file_labels where label_id=?");
    query->addBindValue(labelId);
    query->exec();
    while (query->next())
    {
        int fileId = query->value(0).toInt();
        FileResult fr;
        getFileResultById(fr, fileId);
        list << fr;
    }
}

bool DBHelper::addLabel(const QString &name, const QString &parentName)
{
    if (name.isEmpty() || name == "请输入" || parentName == "请输入")return false;
    int parentId = getLabelId(parentName);
    if (parentId == 0)return false;

    //若已存在标签，不添加
    int nameId = getLabelId(name, 2);
    if (nameId != 0) return false;

    query->prepare("insert into labels(name, level, parent, is_leaf, type) "
                   "values(:name, :lvl, :parent, :is_leaf, :type)");
    query->bindValue(":name", name);
    query->bindValue(":lvl", 3);
    query->bindValue(":parent", parentId);
    query->bindValue(":is_leaf", true);
    query->bindValue(":type", "keyword");
    return  query->exec();

}

bool DBHelper::removeLabel(const QString &name)
{
    query->prepare("delete from labels where name=? and type='keyword'");
    query->addBindValue(name);
    return query->exec();
}

void DBHelper::abortProgress()
{
    if (working)
    {
        abortFlag = true;
        emit dbInterrupted();
    }
}


