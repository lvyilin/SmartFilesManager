#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "analyser.h"

Analyser::Analyser(DBHelper *dh, QObject *parent) :
    QObject(parent),
    dbHelper(dh)
{
    successCount = failCount = finishCount = 0;
    threadCount = 0;
}

void Analyser::processFileList(const QList<File> &fileList)
{
    if (fileList.isEmpty())
    {
        qDebug() << "[processWorkList] worklist is empty.";
        emit processFinished(0, 0);
        return;
    }
    if (MAX_THREAD_NUM <= threadCount)
    {
        qDebug() << "[processWorkList] thread number exceeds.";
        emit processFinished(0, 0);
        return;
    }
    AnalyserThread *workerThread = new AnalyserThread(dbHelper, fileList, this);
    ++threadCount;
    connect(workerThread, &AnalyserThread::resultReady, this, &Analyser::handleResult);
    connect(workerThread, &AnalyserThread::finished, workerThread, &QObject::deleteLater);
    connect(workerThread, &AnalyserThread::aborted, this, &Analyser::analyserInterrupted);
    connect(this, &Analyser::threadQuit, workerThread, &AnalyserThread::abortProgress);
    connect(this, &Analyser::threadWait, workerThread, &AnalyserThread::wait);
    connect(workerThread, &AnalyserThread::finishOne, this, &Analyser::threadProgressAdded);

    workerThread->start();
    qDebug() << "[Analyser] total threads now: " << threadCount;
}

int Analyser::getThreadCount()
{
    return threadCount;
}

void Analyser::handleResult(int success, int fail)
{
    mutex.lock();
    threadCount--;
    successCount += success;
    failCount += fail;
    mutex.unlock();
    emit analyseProgress(successCount + failCount);
    if (threadCount == 0)
    {
        emit processFinished(successCount, failCount);
        successCount = failCount = 0;
    }
}

void Analyser::analyserInterrupted()
{
    emit interrupted();
}

void Analyser::threadProgressAdded()
{
    emit analyseProgress(++finishCount);
}

void Analyser::quitAll()
{
    qDebug() << "[analyser] start quiting thread...total:" << threadCount;
    emit threadQuit();
    emit threadWait(2000);
}
