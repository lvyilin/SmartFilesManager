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

    bool isFirstTimeUsing() const;
    bool isStartAtBoot() const;
    RunningStrategy getRunningStrategy() const;
    int getCpuTriggerPercent() const;
    QTime &getTimeTriggerPoint();
    int getScanInterval() const;

    void setSettings(bool st, int cpuPct, int intv);
    void setSettings(bool st, const QTime &timeTriPt, int intv);

    QStandardItemModel *pathModel;

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

    QSettings *settings;
    static const QString pathProfix;
};

#endif // CONFIGHELPER_H
