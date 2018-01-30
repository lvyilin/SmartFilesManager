#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "dbhelper.h"
#include <qDebug>

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

bool DBHelper::addFile(const File &file)
{
    query->addBindValue(file.name);
    query->addBindValue(file.format);
    query->addBindValue(file.path);
    query->addBindValue(file.size);
    query->addBindValue(file.createTime.toString(Qt::ISODate));
    query->addBindValue(file.modifyTime.toString(Qt::ISODate));
    query->addBindValue(file.isFinished);
    return query->exec();
}

void DBHelper::addFiles(const QList<File> &filesList)
{
    query->prepare("insert into files (name, format, path, size, create_time, modify_time, is_finished) "
                   "values(:name, :format, :path, :size, :create_time, :modify_time, :is_finished)");
    foreach (File file, filesList)
    {
        if (!addFile(file))
        {
            if (query->exec(QString("update files set name = \"%1\" format = \"%2\" path = \"%3\" size = %4 create_time = %5 modify_time = %6 is_finiehed = %7 where path = \"%8\" ")
                            .arg(file.name)
                            .arg(file.format)
                            .arg(file.path)
                            .arg(file.size)
                            .arg(file.createTime.toString(Qt::ISODate))
                            .arg(file.modifyTime.toString(Qt::ISODate))
                            .arg(file.isFinished)
                            .arg(file.path)))
                qDebug() << "update false" << query->lastError();
        }
    }
}

void DBHelper::cleanFiles()
{
    query->prepare("delete from files");
    if (!query->exec())
        qDebug() << query->lastError();
    else
        qDebug() << "table cleared" ;
}

void DBHelper::close()
{
    db.close();
}

QList<File> &DBHelper::getWorkList(int num)
{
    unfinishedFile.clear();
    if (!query->exec(QString("select * from files where is_finished = 0 and is_valid = 1 limit %1").arg(num)))
    {
        qDebug() << "【getWorkList】 error: " << query->lastError().text();
        return unfinishedFile;
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
        unfinishedFile.append(temp);
    }
    return unfinishedFile;
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

    if (!query->exec("create table if not exists file_labels(file_id integer,label_id integer,FOREIGN KEY(file_id) REFERENCES files(id) on delete cascade on update cascade ,FOREIGN KEY(label_id) REFERENCES labels(id)on delete cascade on update cascade ,constraint pk_t2 primary key (file_id,label_id))"))
        qDebug() << "file_labels create false" << query->lastError().text();
    else
        qDebug() << "table create success";

    if (!query->exec("create table if not exists file_relations(file_a_id integer,file_b_id integer,relation int NOT NULL, relation_type int NOT NULL,FOREIGN KEY(file_a_id) REFERENCES files(id) on delete cascade on update cascade ,FOREIGN KEY(file_b_id) REFERENCES labels(id) on delete cascade on update cascade  ,constraint pk_t2 primary key (file_a_id,file_b_id))"))
        qDebug() << "file_relations create false" << query->lastError().text();
    else
        qDebug() << "table create success";
    if (!query->exec("create table if not exists file_keyword(file_id integer,keyword varchar(255),weight double NOT NULL,FOREIGN KEY(file_id) REFERENCES files(id) on delete cascade on update cascade ,constraint pk_t2 primary key (file_id,keyword))"))
        qDebug() << "file_relations create false" << query->lastError().text();
    else
        qDebug() << "table create success";
    if (!query->exec("PRAGMA foreign_keys = ON"))
        qDebug() << "foreign_keys didn't open" << query->lastError().text();
    else
        qDebug() << "foreign_keys have opened"; //打开外键约束
}


void DBHelper::initLabels()
{
    //TODO: 需要init吗?
    /*if (!query->exec("insert into labels(name,level,is_leaf,type) values(\"格式\",1,0,\"格式视图\")"))
        qDebug() << query->lastError();
    query->exec("insert into labels(name,level,is_leaf,type) values(\"知识图谱\",1,0,\"知识图谱视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"文档\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"图片\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"视频\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"音频\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"压缩包\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"其他\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"TXT文件\",3,3,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"HTML文件\",3,3,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"WORD文档文件\",3,3,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"WPS文件\",3,3,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"POWERPOINT文件\",3,3,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"PDF文件\",3,3,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"INF文件\",3,3,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"BMP文件\",3,4,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"JPG、JPEG文件\",3,4,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"PNG文件\",3,4,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"PSD文件\",3,4,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"ICO文件\",3,4,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"MP3文件\",3,5,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"WAV文件\",3,5,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"ACC文件\",3,5,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"MID文件\",3,5,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"VST文件\",3,5,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"AVI文件\",3,6,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"DAT文件\",3,6,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"ANI文件\",3,6,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"MOV文件\",3,6,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"MNG文件\",3,6,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"JAR文件\",3,7,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"APK文件\",3,7,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"RAR文件\",3,7,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"ZIP文件\",3,7,1,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,type) values(\"IMG文件\",3,7,1,\"格式视图\")");*/
}


void DBHelper::setFinish(const File &file, bool finish)
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

    QString fileId = "";

    query->prepare("select id from files where path=:path");
    query->bindValue(":path", fp.file.path);
    query->exec();
    if (query->next())
    {
        fileId = query->value(0).toString();
    }
    if (fileId == "")
    {
        qDebug() << "cannot find file id" << fp.file.name;
    }

    QMapIterator<QString, double> map(fp.keywords);
    if (!db.transaction())
    {
        qDebug() << "db transaction unsupported?";
    }
    while (map.hasNext())
    {
        map.next();
        QString temp_keyword = map.key();
        double temp_weight = map.value();
        if (!query->exec(
                    QString("insert into file_keyword(file_id,keyword,weight) values(\"%1\", \"%2\", \"%3\")")
                    .arg(fileId).arg(temp_keyword).arg(temp_weight)))
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

    QString fileId = "";

    query->prepare("select id from files where path=:path");
    query->bindValue(":path", fp.file.path);
    query->exec();
    if (query->next())
    {
        fileId = query->value(0).toString();
    }
    if (fileId == "")
    {
        qDebug() << "cannot find file id" << fp.file.name;
    }

    foreach (QString label, labels)
    {
        query->prepare("select * from labels where name=:name");
        query->bindValue(":name", label);
        query->exec();
        QVector<QPair<int, int>> itemGroup; //单个标签在库中可能出现多次，Pair中first=id,second=parent_id
        while (query->next())
        {
            qDebug() << "label: " << query->value(1).toString();
            int labelId = query->value(0).toInt();
            int parentId = query->value(3).toInt();
            QPair<int, int> item(labelId, parentId);
            itemGroup.append(item);
        }
        foreach (auto item, itemGroup)
        {
            int id = item.first;
            int parentId = item.second;
            while (true)
            {
                query->prepare("insert into file_labels(file_id ,label_id) values(:file_id, :label_id)");
                query->bindValue(":file_id", fileId);
                query->bindValue(":label_id", id);
                query->exec();
                if (parentId == 0)break;
                query->prepare("select * from labels where id=:id");
                query->bindValue("id", parentId);
                query->exec();
                if (query->next())
                {
                    id = query->value(0).toInt();
                    parentId = query->value(3).toInt();
                }
                else break;
            }
        }
    }

    mutex.unlock();
}

