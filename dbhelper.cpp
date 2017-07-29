#include "dbhelper.h"
#include <qDebug>

DBHelper::DBHelper(QString &conName, QString &dbName, QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE", conName);
    db.setDatabaseName(dbName);
    if (!db.open())
    {
        qDebug() << "Error: connection with database fail";
        qDebug() << db.lastError().text();
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

    //TODO: 建表

}
