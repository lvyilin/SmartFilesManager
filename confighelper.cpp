#include "confighelper.h"
#include <qDebug>

ConfigHelper::ConfigHelper(QObject *parent) : QObject(parent)
{
    settings = new QSettings(
        "config.ini",
        QSettings::IniFormat,
        this);
    pathModel = new QStandardItemModel();
}

const QString ConfigHelper::pathProfix = "ScanPaths";

void ConfigHelper::readSettings()
{
    firstTimeUsing = settings->value("FirstTimeUsing", QVariant(true)).toBool();
    startAtBoot = settings->value("StartAtBoot", QVariant(false)).toBool();
    onlyOneInstace = settings->value("OnlyOneInstance", QVariant(true)).toBool();
    runningStrategy = static_cast<RunningStrategy>(settings->value("RunningStrategy", QVariant(static_cast<int>(CpuTrigger))).toInt());
    cpuTriggerPercent = settings->value("CpuTriggerPercent", QVariant(10)).toInt();
    timeTriggerPoint = settings->value("TimeTriggerPoint", QVariant(QTime(12, 0))).toTime();
    scanIntervalHours = settings->value("ScanIntervalHours", QVariant(12)).toInt();
    fileIndexFinished = settings->value("FileIndexFinished", QVariant(false)).toBool();
    interruptionType = static_cast<InterruptionType>(settings->value("InterruptionType", QVariant(static_cast<int>(NoInterrupt))).toInt());
    autoCalculateRelation = settings->value("AutoCalRelation", QVariant(true)).toBool();
    //detact termination
    settings->setValue("InterruptionType", QVariant(static_cast<int>(TerminateInterrupt)));

    int size = settings->beginReadArray(pathProfix);
    for (int i = 0; i < size; i++)
    {
        settings->setArrayIndex(i);
        QVariant value = settings->value("Path");
        QStandardItem *path = new QStandardItem(value.toString());
        path->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        pathModel->setItem(i, path);
    }
    settings->endArray();
}

void ConfigHelper::saveSettings()
{
    settings->setValue("FirstTimeUsing", QVariant(false));
    settings->setValue("StartAtBoot", QVariant(startAtBoot));
    settings->setValue("OnlyOneInstance", QVariant(onlyOneInstace));
    settings->setValue("RunningStrategy", QVariant(runningStrategy));
    settings->setValue("CpuTriggerPercent", QVariant(static_cast<int>(cpuTriggerPercent)));
    settings->setValue("TimeTriggerPoint", QVariant(timeTriggerPoint));
    settings->setValue("ScanIntervalHours", QVariant(scanIntervalHours));
    settings->setValue("AutoCalRelation", QVariant(autoCalculateRelation));

    int pathSize = pathModel->rowCount();
    qDebug() << "[saveSettings] path size: " << pathSize;
    settings->beginWriteArray(pathProfix);
    for (int i = 0; i < pathSize; i++)
    {
        settings->setArrayIndex(i);
        settings->setValue("Path", QVariant(QString(pathModel->item(i)->text())));
    }
    settings->endArray();

}

bool ConfigHelper::isFirstTimeUsing() const
{
    return firstTimeUsing;
}

bool ConfigHelper::isStartAtBoot() const
{
    return startAtBoot;
}

RunningStrategy ConfigHelper::getRunningStrategy() const
{
    return runningStrategy;
}

int ConfigHelper::getCpuTriggerPercent() const
{
    return cpuTriggerPercent;
}

QTime &ConfigHelper::getTimeTriggerPoint()
{
    return timeTriggerPoint;
}

int ConfigHelper::getScanInterval() const
{
    return scanIntervalHours;
}

void ConfigHelper::setSettings(bool st, int cpuPct, int intv, bool autoCal)
{
    startAtBoot = st;
    cpuTriggerPercent = cpuPct;
    scanIntervalHours = intv;
    runningStrategy = CpuTrigger;
    autoCalculateRelation = autoCal;
}

void ConfigHelper::setSettings(bool st, const QTime &timeTriPt, int intv, bool autoCal)
{
    startAtBoot = st;
    timeTriggerPoint = timeTriPt;
    scanIntervalHours = intv;
    runningStrategy = TimeTrigger;
    autoCalculateRelation = autoCal;
}

bool ConfigHelper::hasLastInterrupted() const
{
    return !(interruptionType == NoInterrupt);
}

InterruptionType ConfigHelper::getInterruptionType() const
{
    return interruptionType;
}

void ConfigHelper::setInterruptionType(InterruptionType it)
{
    interruptionType = it;
}

void ConfigHelper::close()
{
    settings->setValue("InterruptionType", QVariant(static_cast<int>(interruptionType)));
    settings->setValue("FileIndexFinished", QVariant(fileIndexFinished));
    qDebug() << "[ConfigHelper] Interruption type: " << static_cast<int>(interruptionType);
}

void ConfigHelper::setFileIndexFinished(bool value)
{
    fileIndexFinished = value;
}

bool ConfigHelper::isFileIndexFinished() const
{
    return fileIndexFinished;
}

bool ConfigHelper::isAutoCalRelation() const
{
    return autoCalculateRelation;
}
