#include "relationcalculator.h"
#include "relationcalculatetask.h"
#include <QDebug>

RelationCalculator::RelationCalculator(DBHelper *db, QObject *parent):
    dbHelper(db),
    QObject(parent)
{
    pool = QThreadPool::globalInstance();
}

void RelationCalculator::start()
{
    fileResultList.clear();
    dbHelper->getFinishedFileResults(fileResultList);
    QList<FileResult> li = fileResultList;
    for (int i = fileResultList.count() - 1; i > 0; --i)
    {
        li.removeLast();
        RelationCalculateTask *task = new RelationCalculateTask(&fileResultList[i], li);
        connect(task, &RelationCalculateTask::finished, this, &RelationCalculator::singleTaskFinished);
        ++threadCount;
        pool->start(task);
    }

}

bool RelationCalculator::isFinished()
{
    return threadCount == 0;
}

void RelationCalculator::singleTaskFinished()
{
    --threadCount;
    if (threadCount == 0)
    {
        saveResult();
        emit allTasksFinished();
    }
}

void RelationCalculator::saveResult()
{
    dbHelper->saveFileResults(fileResultList);
}
