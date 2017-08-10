#include "dbhelper.h"
#include <qDebug>

DBHelper::DBHelper(QString &conName, QString &dbName, QObject *parent) : QObject(parent)
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

bool DBHelper::hasIndex()
{
    //0810@YL: 需求更改，需修改
    query->exec("SELECT name FROM paths");
    if (!query->next())
    {
        qDebug() << "hasIndex(): false";
        return false;
    }
    return true;
}

void DBHelper::addFile(File &file)
{
    //0810@YL: 需求更改，需修改
    //    query->bindValue(":name", file);
    //    query->exec();
    //    qDebug() << "add File result:" << res << query->lastError();
}

void DBHelper::addFiles(QList<File> &filesList)
{
    //0810@YL: 需求更改，需修改
    //    query->prepare("insert into paths(id,name) "
    //                   "values(:id, :name)");
    //    foreach (QString Path, pathSet)
    //    {
    //        addFile(Path);
    //    }
}

void DBHelper::cleanFiles()
{
    //0810@YL：改动：删除所有条目，不删除表
    query->exec("DELETE * FROM paths");
}

void DBHelper::close()
{
    db.close();
}

QList<File> DBHelper::getFiles()
{
    //0810@YL: 需求变动，需修改

    //    QSet<QString> temp;
    //    query->exec("SELECT name FROM paths");
    //    qDebug() << "Paths in db:";
    //    while (query->next())
    //    {
    //        QString s = query->value(0).toString();
    //        temp.insert(s);
    //        qDebug() << s;
    //    }
    //    return temp;
    return QList<File>();
}

void DBHelper::createTable()
{
    //0810@YL: 使用create table if not exists代替(待定)

    //    QStringList tables = db.tables();
    //    if (tables.contains("paths", Qt::CaseInsensitive))
    //    {
    //        qDebug() << "table had created before";
    //    }
    if (!query->exec("create table if not exists paths(id integer primary key autoincrement, name varchar(255) not null)"))
    {
        qDebug() << "table create false" << query->lastError().text();
    }
    else qDebug() << "table create success";

    if(!query->exec("create table if not exists labels("
                    "id integer primary key autoincrement NOT NULL,"
                    "name varchar(255) NOT NULL,"
                    "parent int NOT NULL,"
                    "is_leaf bool NOT NULL,"
                    "view_type varchar(255) NOT NULL"
                    ")"))
        qDebug() << "labels create false"<< query->lastError().text();
        else
        qDebug() << "table create success";

    if(!query->exec("create table if not exists files("
                    "id integer primary key autoincrement NOT NULL,"
                    "name varchar(255) NOT NULL,"
                    "format varchar(10) NOT NULL,"
                    "path varchar(255) NOT NULL,"
                    "size "
                    "unsigned big int NOT NULL,"
                    "create_time DATATIME NOT NULL,"
                    "modify_time DATATIME NOT NULL,"
                    "is_finished bool NOT NULL)"))
        qDebug() << "files create false"<< query->lastError().text();
        else
        qDebug() << "table create success";

    if(!query->exec("create table if not exists file_labels(files_id integer,label_id integer,FOREIGN KEY(files_id) REFERENCES files(id),FOREIGN KEY(label_id) REFERENCES labels(id),constraint pk_t2 primary key (files_id,label_id))"))
        qDebug() << "file_labels create false"<< query->lastError().text();
        else
        qDebug() << "table create success";

    if(!query->exec("create table if not exists file_relation(relation int NOT NULL, relation_type int NOT NULL, "
                    "files_id integer,label_id integer,FOREIGN KEY(files_id) REFERENCES files(id),FOREIGN KEY(label_id) REFERENCES labels(id),constraint pk_t2 primary key (files_id,label_id))"))
        qDebug() << "file_relations create false"<< query->lastError().text();
        else
        qDebug() << "table create success";
        query->exec("PRAGMA foreign_keys = ON"); //打开外键约束
}
