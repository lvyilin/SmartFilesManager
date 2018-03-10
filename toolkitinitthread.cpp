#include "toolkitinitthread.h"
#include "toolkit.h"

ToolkitInitThread::ToolkitInitThread(QObject *parent) : QThread(parent)
{

}

void ToolkitInitThread::run()
{
    emit startInit();
    Toolkit::getInstance();
    emit finishInit();
}
