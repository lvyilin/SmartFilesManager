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
    if(!query.exec("create table Paths(id integer PRIMARY KEY AUTOINCREMENT, name varchar(255) NOT NULL)"))
        qDebug()<<"false";//判断表格是否建立成功
}

bool DBHelper::hasIndex()
{
    if(!query.next())
        return false;  //判断表格是否已经存在
    return true;
}

void DBHelper::addFile(QString &path)
{ 
    query.exec(QString("INSERT INTO Paths(%1)").arg(path));
}

void DBHelper::addFiles(QSet<QString> &pathSet)
{
    foreach (QString Path , pathSet)
    {
        addFile(Path);
    }
}

QSet<QString> DBHelper::getFiles()
{
    QSet<QString> temp;
    QSqlQuery query("SELECT name FROM Paths");
        while (query.next())
        {
            QString country = query.value(0).toString();
            temp.insert(country);
        }
        return temp;
}
