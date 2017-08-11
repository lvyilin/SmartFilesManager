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
    query->exec("SELECT name FROM files");
    if (!query->next())
    {
        return false;
    }
    return true;
}

void DBHelper::addFile(File &file)
{
    query->addBindValue(file.name);
    query->addBindValue(file.format);
    query->addBindValue(file.path);
    query->addBindValue(file.size);
    query->addBindValue(file.createTime.toString(Qt::ISODate));
    query->addBindValue(file.modifyTime.toString(Qt::ISODate));
    query->addBindValue(file.isFinished);

    query->exec();
}

void DBHelper::addFiles(QList<File> &filesList)
{
    query->prepare("insert into files (name, format, path, size, create_time, modify_time, is_finished) "
                   "values(:name, :format, :path, :size, :create_time, :modify_time, :is_finished)");
    foreach (File file, filesList)
    {
        addFile(file);
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

QList<File> &DBHelper::getWorkList(QString format, int num)
{
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
                     "parent int NOT NULL,"
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
                     "path varchar(255) NOT NULL,"
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
