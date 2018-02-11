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
        connect(task, &RelationCalculateTask::finished, this, &RelationCalculator::singleTaskFinished);
        ++threadCount;
        pool->start(task);
        qDebug() << "start relation calculate:" << fileResultList[i].file.name;
    }
}

bool RelationCalculator::isFinished()
{
    return threadCount == 0;
}

void RelationCalculator::singleTaskFinished(FileResult *fr)
{
    --threadCount;
    qDebug() << "finish one task, task count: " << threadCount;
    dbHelper->saveSingleFileResult(*fr);
    if (threadCount == 0)
    {
//        saveResult();
        emit allTasksFinished();
    }
}

bool RelationCalculator::isSupportedFormat(const QString &format)
{
    return (DOCUMENT_FORMATS.contains(format, Qt::CaseInsensitive)) ||
           (IMAGE_FORMATS.contains(format, Qt::CaseInsensitive)) ||
           (AUDIO_FORMATS.contains(format, Qt::CaseInsensitive)) ||
           (VIDEO_FORMATS.contains(format, Qt::CaseInsensitive));
}

void RelationCalculator::saveResult()
{
    dbHelper->saveFileResults(fileResultList);
}
