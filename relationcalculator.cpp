#include "relationcalculator.h"
#include <QDebug>
#include <QCoreApplication>

RelationCalculator::RelationCalculator(DBHelper *db, QObject *parent):
    dbHelper(db),
    QThread(parent)
{
}

RelationCalculator::~RelationCalculator()
{
    for (auto task : taskList)
        delete task;
}

void RelationCalculator::run()
{
    emit startTask();
    finished = false;
    if (!fileResultList.isEmpty())
    {
        for (auto task : taskList)
            delete task;
    }
    fileResultList.clear();
    taskList.clear();
    dbHelper->getFileResults(fileResultList);
    for (int i = fileResultList.count() - 1; i > 0; --i)
    {
        //reduce size of list
        //
        QList<FileResult> li = fileResultList;
        for (int j = 0; j < fileResultList.count() - i; ++j)
            li.removeLast();
        //reduce item which had built relation
        for (Relation &relationAlready : fileResultList[i].relations)
        {
            int k = li.count() - 1;
            for (; k >= 0; --k)
            {
                if (relationAlready.file.path == li[k].file.path)
                    li.removeLast();
            }
        }

        RelationCalculateTask *task = new RelationCalculateTask(&fileResultList[i], li);
        //        connect()
        taskList << task;
        task->run();
    }
    taskFinished();
}

bool RelationCalculator::isFinished()
{
    return finished;
}

void RelationCalculator::taskFinished()
{
    dbHelper->saveFileResults(fileResultList);
    emit allTasksFinished();
}

void RelationCalculator::newTaskOk()
{
    finished = true;
}

bool RelationCalculator::isSupportedFormat(const QString &format)
{
    return (DOCUMENT_FORMATS.contains(format, Qt::CaseInsensitive)) ||
           (IMAGE_FORMATS.contains(format, Qt::CaseInsensitive)) ||
           (AUDIO_FORMATS.contains(format, Qt::CaseInsensitive)) ||
           (VIDEO_FORMATS.contains(format, Qt::CaseInsensitive));
}
