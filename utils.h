#ifndef UTILS_H
#define UTILS_H

#include <QDateTime>
#include <QMap>

struct File
{
    QString name;
    QString format;
    QString path;
    qint64 size;
    QDateTime createTime;
    QDateTime modifyTime;
    bool isFinished;
    bool isValid;
};

struct FileProduct
{
    File file;
    QStringList contents;
    QMap<QString, int> keywords;
};

enum RunningStrategy
{
    CpuTrigger, TimeTrigger
};

enum InterruptionType
{
    NoInterrupt, AnalyserInterrupt, FileUpdaterInterrupt, TerminateInterrupt
};

const int MAX_THREAD_COUNT = 10;

#endif // UTILS_H
