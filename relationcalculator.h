#ifndef RELATIONCALCULATOR_H
#define RELATIONCALCULATOR_H

#include <QThread>
#include "utils.h"
#include "dbhelper.h"
#include "relationcalculatetask.h"

class RelationCalculator : public QThread {
    Q_OBJECT
public:
    explicit RelationCalculator(DBHelper *db, QObject *parent = nullptr);
    ~RelationCalculator();
    void run() override;
    bool isFinished();
signals:
    void startTask();
    void allTasksFinished();
public slots:
    void taskFinished();
    void newTaskOk();

private:
    bool finished;
    DBHelper *dbHelper;
    QList<FileResult> fileResultList;
    QList<RelationCalculateTask *> taskList;

    bool isSupportedFormat(const QString &format);
};

#endif // RELATIONCALCULATOR_H
