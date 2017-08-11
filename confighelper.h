#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include <QStringList>
#include <QSettings>
#include <QTime>
#include <QStandardItemModel>

class ConfigHelper : public QObject {
    Q_OBJECT
public:
    enum RunningStrategy
    {
        CpuTrigger, TimeTrigger
    };

    explicit ConfigHelper(QObject *parent = nullptr);

    void readSettings();
    void saveSettings();

    bool isStartAtBoot();
    RunningStrategy getRunningStrategy();
    int getCpuTriggerPercent();
    QTime &getTimeTriggerPoint();
    int getScanInterval();

    void setSettings(bool st, int cpuPct, int intv);
    void setSettings(bool st, QTime timeTriPt, int intv);


    QStandardItemModel *pathModel;

signals:
public slots:

private:
    bool startAtBoot;
    bool onlyOneInstace;//TODO:check if only one instance
    RunningStrategy runningStrategy;
    int cpuTriggerPercent;
    QTime timeTriggerPoint;
    int scanIntervalHours;

    QSettings *settings;
    static const QString pathProfix;
};

#endif // CONFIGHELPER_H
