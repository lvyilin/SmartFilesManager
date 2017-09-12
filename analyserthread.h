﻿#ifndef ANALYSERTHREAD_H
#define ANALYSERTHREAD_H

#include <QThread>
#include "dbhelper.h"
//#include "analyser.h"
#include "JlCompress.h"
#include <QFile>
#include <QDomDocument>
#include <QString>
#include <QMimeDatabase>
#include <QMimeType>
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
    QMimeDatabase mimeDb;
    QMutex mutex;
    bool abortFlag;

    QString docxExtract(const File &file);
    bool processFile(const File &file);
    bool isSupportedFormat(QString format) const;
};

#endif // ANALYSERTHREAD_H