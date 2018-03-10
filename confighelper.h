#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include "utils.h"
#include <QStringList>
#include <QSettings>
#include <QTime>
#include <QStandardItemModel>

class ConfigHelper : public QObject {
    Q_OBJECT
public:
    explicit ConfigHelper(QObject *parent = nullptr);

    void readSettings();
    void saveSettings();

    bool isFirstTimeUsing() const;
    bool isStartAtBoot() const;
    RunningStrategy getRunningStrategy() const;
    int getCpuTriggerPercent() const;
    QTime &getTimeTriggerPoint();
    int getScanInterval() const;

    void setSettings(bool st, int cpuPct, int intv, bool autoCal, int edgePct);
    void setSettings(bool st, const QTime &timeTriPt, int intv, bool autoCal, int edgePct);

    bool hasLastInterrupted() const;
    InterruptionType getInterruptionType() const;
    void setInterruptionType(InterruptionType it);

    void close();

    QStandardItemModel *pathModel;

    void setFileIndexFinished(bool value);
    bool isFileIndexFinished() const;
    bool isAutoCalRelation() const;

    int getDisplayEdgePercent() const;

signals:
public slots:

private:
    bool firstTimeUsing;
    bool startAtBoot;
    bool onlyOneInstace;//TODO:check if only one instance
    RunningStrategy runningStrategy;
    int cpuTriggerPercent;
    QTime timeTriggerPoint;
    int scanIntervalHours;
    bool fileIndexFinished;
    int displayEdgePercent;

    QSettings *settings;
    static const QString pathProfix;

    InterruptionType interruptionType;

    bool autoCalculateRelation;

    void setupStartAtBootState(bool start);
};

#endif // CONFIGHELPER_H
