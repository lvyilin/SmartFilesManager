#ifndef RELATIONCALCULATOR_H
#define RELATIONCALCULATOR_H

#include <QThreadPool>
#include "utils.h"
#include "dbhelper.h"

class RelationCalculator : public QObject {
    Q_OBJECT
public:
    explicit RelationCalculator(DBHelper *db, QObject *parent = nullptr);
    void start();
    bool isFinished();
signals:
    void allTasksFinished();
public slots:
    void singleTaskFinished(FileResult *fr);

private:
    QThreadPool *pool;
    DBHelper *dbHelper;
    QList<FileResult> fileResultList;

    int threadCount = 0;
    bool isSupportedFormat(const QString &format);
    void saveResult();
};

#endif // RELATIONCALCULATOR_H
