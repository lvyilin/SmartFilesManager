#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "analyser.h"

Analyser::Analyser(DBHelper *dh, QObject *parent) :
    QObject(parent),
    dbHelper(dh)
{
    successCount = failCount = 0;
    threadCount = 0;
}



QStringList Analyser::getSupportedFormatsList() const
{
    return supportedFormat;
}

QStringList Analyser::getSupportedFormatsFilter() const
{
    return supportedFormatFilter;
}

void Analyser::processFileList(const QList<File> &fileList)
{
    successCount = failCount = 0;
    AnalyserThread *workerThread = new AnalyserThread(dbHelper, getSupportedFormatsList(), fileList, this);
    connect(workerThread, &AnalyserThread::resultReady, this, &Analyser::handleResult);
    connect(workerThread, &AnalyserThread::finished, workerThread, &QObject::deleteLater);
    threadCount++;
    qDebug() << "[Analyser] total threads: " << threadCount;
    workerThread->start();
}

void Analyser::handleResult(int success, int fail)
{
    successCount += success;
    failCount += fail;
    threadCount--;
    if (threadCount == 0)
    {
        emit processFinished(successCount, failCount);
    }
}

