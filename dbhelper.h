#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>

class DBHelper : public QObject {
    Q_OBJECT
public:
    explicit DBHelper(QString &conName, QString &dbName, QObject *parent = nullptr);

signals:

public slots:
private:
    QSqlDatabase db;
};

#endif // DBHELPER_H
