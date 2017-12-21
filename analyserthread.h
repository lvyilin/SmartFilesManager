#ifndef ANALYSERTHREAD_H
#define ANALYSERTHREAD_H

#include <QThread>
#include "utils.h"
#include "dbhelper.h"
#include "JlCompress.h"
#include <QFile>
#include <QDomDocument>
#include <QString>
#include <QMutex>

class AnalyserThread : public QThread {
    Q_OBJECT
public:
    explicit AnalyserThread(DBHelper *db, const QStringList &li, const QList<File> &f, QObject *parent = nullptr);
    void run() override;

signals:
    void resultReady(int success, int fail);
    void aborted();

public slots:
    void abortProgress();

private:
    DBHelper *dbHelper;
    QList<File> fileList;
    QStringList supportedFormat;
    QMutex mutex;
    bool abortFlag;

    QString docxExtract(const File &file);
    ProcessingResult processFile(const File &file);
    void generateKeywords(FileProduct &fpd);
};

#endif // ANALYSERTHREAD_H
