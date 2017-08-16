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
    query->exec("SELECT name FROM files");
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
            query->exec(QString("delete from files where path = \"%1\"").arg(file.path));
            query->prepare("insert into files (name, format, path, size, create_time, modify_time, is_finished) "
                           "values(:name, :format, :path, :size, :create_time, :modify_time, :is_finished)");
            if (!addFile(file))
                qDebug() << "update false" << query->lastError() ;
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

QList<File> &DBHelper::getWorkList(const QString &format, int num)
{
    unfinishedFile.clear();
    int i = 0;
    if (!query->exec(QString("select * from files where is_finished = 0 and format = \"%1\"").arg(format)))
    {
        qDebug() << "【getWorkList】 error: " << query->lastError().text();
        return unfinishedFile;
    }
    while (query->next() && i++ < num)
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
                     "level int NOT NULL,"
                     "parent int,"
                     "is_leaf bool NOT NULL,"
                     "view_type varchar(255) NOT NULL"
                     ")"))
        qDebug() << "labels create false" << query->lastError().text();
    else
        qDebug() << "table create success";

    if (!query->exec("create table if not exists files("
                     "id integer primary key autoincrement NOT NULL,"
                     "name varchar(255) NOT NULL,"
                     "format varchar(10) NOT NULL,"
                     "path varchar(255) NOT NULL UNIQUE,"
                     "size "
                     "unsigned big int NOT NULL,"
                     "create_time DATETIME NOT NULL,"
                     "modify_time DATETIME NOT NULL,"
                     "is_finished BOOLEAN NOT NULL)"))
        qDebug() << "files create false" << query->lastError().text();
    else
        qDebug() << "table create success";

    if (!query->exec("create table if not exists file_labels(files_id integer,label_id integer,FOREIGN KEY(files_id) REFERENCES files(id) on delete cascade on update cascade  ,FOREIGN KEY(label_id) REFERENCES labels(id)on delete cascade on update cascade ,constraint pk_t2 primary key (files_id,label_id))"))
        qDebug() << "file_labels create false" << query->lastError().text();
    else
        qDebug() << "table create success";

    if (!query->exec("create table if not exists file_relations(file_a_id integer,file_b_id integer,relation int NOT NULL, relation_type int NOT NULL,FOREIGN KEY(file_a_id) REFERENCES files(id) on delete cascade on update cascade ,FOREIGN KEY(file_b_id) REFERENCES labels(id) on delete cascade on update cascade  ,constraint pk_t2 primary key (file_a_id,file_b_id))"))
        qDebug() << "file_relations create false" << query->lastError().text();
    else
        qDebug() << "table create success";
    query->exec("PRAGMA foreign_keys = ON"); //打开外键约束
}

void DBHelper::initLabels()
{
    if (!query->exec("insert into labels(name,level,is_leaf,view_type) values(\"格式\",1,0,\"格式视图\")"))
        qDebug() << query->lastError();
    query->exec("insert into labels(name,level,is_leaf,view_type) values(\"知识图谱\",1,0,\"知识图谱视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"文档\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"图片\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"视频\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"音频\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"压缩包\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"其他\",2,1,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"TXT文件\",3,3,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"HTML文件\",3,3,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"WORD文档文件\",3,3,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"WPS文件\",3,3,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"POWERPOINT文件\",3,3,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"PDF文件\",3,3,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"INF文件\",3,3,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"BMP文件\",3,4,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"JPG、JPEG文件\",3,4,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"PNG文件\",3,4,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"PSD文件\",3,4,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"ICO文件\",3,4,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"MP3文件\",3,5,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"WAV文件\",3,5,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"ACC文件\",3,5,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"MID文件\",3,5,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"VST文件\",3,5,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"AVI文件\",3,6,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"DAT文件\",3,6,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"ANI文件\",3,6,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"MOV文件\",3,6,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"MNG文件\",3,6,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"JAR文件\",3,7,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"APK文件\",3,7,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"RAR文件\",3,7,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"ZIP文件\",3,7,0,\"格式视图\")");
    query->exec("insert into labels(name,level,parent,is_leaf,view_type) values(\"IMG文件\",3,7,0,\"格式视图\")");
}

void DBHelper::setFinish(const File &file, bool finish)
{

}

void DBHelper::setValid(const File &file, bool valid)
{

}
