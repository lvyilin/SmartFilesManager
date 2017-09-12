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
    if (MAX_THREAD_COUNT <= threadCount)
    {
        qDebug() << "[processWorkList] thread number exceeds.";
        emit processFinished(0, 0);
        return;
    }
    successCount = failCount = 0;
    AnalyserThread *workerThread = new AnalyserThread(dbHelper, getSupportedFormatsList(), fileList, this);
    ++threadCount;
    connect(workerThread, &AnalyserThread::resultReady, this, &Analyser::handleResult);
    connect(workerThread, &AnalyserThread::finished, workerThread, &QObject::deleteLater);
    connect(workerThread, &AnalyserThread::aborted, this, &Analyser::analyserInterrupted);
    connect(this, &Analyser::threadQuit, workerThread, &AnalyserThread::abortProgress);
    connect(this, &Analyser::threadWait, workerThread, &AnalyserThread::wait);
    workerThread->start();
    qDebug() << "[Analyser] total threads now: " << threadCount;
}

void Analyser::handleResult(int success, int fail)
{
    mutex.lock();
    threadCount--;
    successCount += success;
    failCount += fail;
    mutex.unlock();
    //BUG:可能出现提示多次的情况
    if (threadCount == 0)
    {
        emit processFinished(successCount, failCount);
    }
}

void Analyser::analyserInterrupted()
{
    emit interrupted();
}

void Analyser::quitAll()
{
    qDebug() << "[analyser] start quiting thread...total:" << threadCount;
    emit threadQuit();
    emit threadWait(2000);
    /* qDebug() << "【】" << (*threadList[0])->isRunning();
     for (int i = 0; i < threadCount; i++)
     {
         if (threadList[i] && (*threadList[i])->isRunning())
         {
             (*threadList[i])->quit();
         }
     }*/
}
