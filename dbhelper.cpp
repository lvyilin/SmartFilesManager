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
    createPathsTable();
}

bool DBHelper::hasIndex()
{
    query->exec("SELECT name FROM paths");
    if (!query->next())
    {
        qDebug() << "hasIndex(): false";
        return false;  //判断表格是否已经存在
    }
    return true;

}

void DBHelper::addFile(QString &path)
{
    query->bindValue(":name", path);
    bool res = query->exec();
    //    qDebug() << "add File result:" << res << query->lastError();
}

void DBHelper::addFiles(QSet<QString> &pathSet)
{
    query->prepare("insert into paths(id,name) "
                   "values(:id, :name)");
    foreach (QString Path, pathSet)
    {
        addFile(Path);
    }
}

void DBHelper::close()
{
    db.close();
}

QSet<QString> DBHelper::getFiles()
{
    QSet<QString> temp;
    query->exec("SELECT name FROM paths");
    qDebug() << "Paths in db:";
    while (query->next())
    {
        QString s = query->value(0).toString();
        temp.insert(s);
        qDebug() << s;
    }
    return temp;
}

void DBHelper::createPathsTable()
{
    QStringList tables = db.tables();
    if (tables.contains("paths", Qt::CaseInsensitive))
    {
        qDebug() << "table had created before";
    }
    else
    {
        if (!query->exec("create table paths(id integer primary key autoincrement, name varchar(255) not null)"))
        {
            qDebug() << "table create false" << query->lastError().text();
        }
        else qDebug() << "table create success";
    }
}
