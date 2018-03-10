#ifndef TOOLKITINITTHREAD_H
#define TOOLKITINITTHREAD_H

#include <QThread>

class ToolkitInitThread : public QThread {
    Q_OBJECT
public:
    explicit ToolkitInitThread(QObject *parent = nullptr);
    void run() override;

signals:
    void startInit();
    void finishInit();

public slots:

};

#endif // TOOLKITINITTHREAD_H
