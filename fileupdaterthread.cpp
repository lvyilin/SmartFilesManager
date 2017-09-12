#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "fileupdaterthread.h"
#include <QDirIterator>
#include <QDebug>

FileUpdaterThread::FileUpdaterThread(DBHelper *db, QStringList f, QStringList p, QObject *parent):
    dbHelper(db),
    formatList(f),
    pathList(p),
    QThread(parent)
{
    abortFlag = false;
}

const int MAX_FILES_NUMBER = 5000;
void FileUpdaterThread::run()
{
    int filesCount = 0;
    foreach (QString path, pathList)
    {
        QDirIterator it(path,
                        formatList,
                        QDir::Files,
                        QDirIterator::Subdirectories);
        while (!abortFlag && it.hasNext() && filesCount < MAX_FILES_NUMBER)
        {
            emit findFilesProgress(filesCount);
            QString thisPath = it.next();
            QFileInfo thisInfo(thisPath);

            File thisFile;
            thisFile.path = thisPath;
            thisFile.name = thisInfo.fileName();
            thisFile.format = thisInfo.suffix().toLower();
            thisFile.createTime = thisInfo.created();
            thisFile.modifyTime = thisInfo.lastModified();
            thisFile.size = thisInfo.size();
            thisFile.isFinished = false;
            thisFile.isValid = true;

            filesList << thisFile;
            ++filesCount;
        }
        if (!abortFlag && MAX_FILES_NUMBER == filesCount)
            break;
    }
    if (abortFlag)
        return;
    if (MAX_FILES_NUMBER == filesCount)
    {
        emit resultReady(tr("操作中断! 文件数超过最大限额: %1, 前往\"设置\"更改路径")
                         .arg(MAX_FILES_NUMBER));
    }
    else
    {
        emit startDbProgress();
        dbHelper->addFiles(filesList);

        qDebug() << "[updateFilesList] files num: " << filesList.count();
        emit resultReady(tr("更新文件列表完成! 操作了%1个文件.").arg(filesCount));
    }
}

void FileUpdaterThread::abortProgress()
{
    abortFlag = true;
    emit aborted();
}
