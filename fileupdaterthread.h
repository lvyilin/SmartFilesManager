#ifndef FILEUPDATERTHREAD_H
#define FILEUPDATERTHREAD_H

#include <QThread>
#include <QStringList>
#include "dbhelper.h"
class FileUpdaterThread : public QThread {
    Q_OBJECT
public:
    explicit FileUpdaterThread(DBHelper *db,
                               QStringList f,
                               QStringList p,
                               QObject *parent = nullptr);
    void run() override;

signals:
    void findFilesProgress(int num);
    void startDbProgress();
    void resultReady(const QString &s);

private:
    DBHelper *dbHelper;
    QStringList formatList;
    QStringList pathList;
    QList<File> filesList;
};

#endif // FILEUPDATERTHREAD_H
