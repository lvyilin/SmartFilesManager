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
    QString contents;
    QMap<QString, double> keywords;
};

enum RunningStrategy
{
    CpuTrigger, TimeTrigger
};

enum InterruptionType
{
    NoInterrupt, AnalyserInterrupt, FileUpdaterInterrupt, TerminateInterrupt
};

enum ProcessingResult
{
    NoException,
    ProcessAborted,
    FileNotFoundException,
    FileAccessException,
    FileReadException,
    FileFormatNotSupported,
    DocxExtractException
};

//Toolkit
const size_t NEEDED_TOP_KEYWORDS = 20;
const double FILENAME_WEIGHTED_VARIANCE = 2.0;
//MainWindow
const int WORKLIST_SIZE = 100;
const int MAX_WORKLIST_NUM = 10;
//Analyser
const int MAX_THREAD_NUM = MAX_WORKLIST_NUM;
//FileUpdater
const int MAX_FILES_NUMBER = 5000;

#endif // UTILS_H
