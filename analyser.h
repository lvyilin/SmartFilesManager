#ifndef ANALYSER_H
#define ANALYSER_H

#include <QObject>
#include "analyserthread.h"
#include "utils.h"
#include <QFile>
#include <qDebug>
#include <QList>
#include <QMutex>

class Analyser : public QObject {
    Q_OBJECT
public:
    explicit Analyser(DBHelper *dh, QObject *parent = nullptr);

    QStringList getSupportedFormatsList() const;
    QStringList getSupportedFormatsFilter() const;
    void processFileList(const QList<File> &fileList);
    int getThreadCount();
    void quitAll();
signals:
    void processFinished(int sc, int fc);
    void threadQuit();
    void threadWait(unsigned long time);
    void interrupted();

public slots:
    void handleResult(int success, int fail);
    void analyserInterrupted();

private:
    const QStringList supportedFormat = {"docx", "txt"};
    const QStringList supportedFormatFilter = {"*.docx", "*.txt"};
    DBHelper *dbHelper;
    int successCount;
    int failCount;
    int threadCount;
    QMutex mutex;

};

#endif // ANALYSER_H
