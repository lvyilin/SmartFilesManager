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

    startAtBoot = settings->value("StartAtBoot", QVariant(false)).toBool();
    onlyOneInstace = settings->value("OnlyOneInstance", QVariant(true)).toBool();
    runningStrategy = static_cast<RunningStrategy>(settings->value("RunningStrategy", QVariant(static_cast<int>(CpuTrigger))).toInt());
    cpuTriggerPercent = settings->value("CpuTriggerPercent", QVariant(10)).toInt();
    timeTriggerPoint = settings->value("TimeTriggerPoint", QVariant(QTime(12, 0))).toTime();
    scanIntervalHours = settings->value("ScanIntervalHours", QVariant(12)).toInt();

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
    settings->setValue("StartAtBoot", QVariant(startAtBoot));
    settings->setValue("OnlyOneInstance", QVariant(onlyOneInstace));
    settings->setValue("RunningStrategy", QVariant(runningStrategy));
    settings->setValue("CpuTriggerPercent", QVariant(static_cast<int>(cpuTriggerPercent)));
    settings->setValue("TimeTriggerPoint", QVariant(timeTriggerPoint));
    settings->setValue("ScanIntervalHours", QVariant(scanIntervalHours));

    int pathSize = pathModel->rowCount();
    qDebug() << pathSize;
    settings->beginWriteArray(pathProfix);
    for (int i = 0; i < pathSize; i++)
    {
        settings->setArrayIndex(i);
        settings->setValue("Path", QVariant(QString(pathModel->item(i)->text())));
    }
    settings->endArray();

}

bool ConfigHelper::isStartAtBoot()
{
    return startAtBoot;
}

ConfigHelper::RunningStrategy ConfigHelper::getRunningStrategy()
{
    return runningStrategy;
}

int ConfigHelper::getCpuTriggerPercent()
{
    return cpuTriggerPercent;
}

QTime ConfigHelper::getTimeTriggerPoint()
{
    return timeTriggerPoint;
}

int ConfigHelper::getScanInterval()
{
    return scanIntervalHours;
}

void ConfigHelper::setSettings(bool st, int cpuPct, int intv)
{
    startAtBoot = st;
    cpuTriggerPercent = cpuPct;
    scanIntervalHours = intv;
    runningStrategy = CpuTrigger;

}

void ConfigHelper::setSettings(bool st, QTime timeTriPt, int intv)
{
    startAtBoot = st;
    timeTriggerPoint = timeTriPt;
    scanIntervalHours = intv;
    runningStrategy = TimeTrigger;

}
