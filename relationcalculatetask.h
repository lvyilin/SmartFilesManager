#ifndef RELATIONCALCULATETASK_H
#define RELATIONCALCULATETASK_H

#include <QRunnable>
#include <QObject>
#include "utils.h"

class RelationCalculateTask : public QObject, public QRunnable {
    Q_OBJECT
public:
    explicit RelationCalculateTask(FileResult *fr,
                                   QList<FileResult> li,
                                   QObject *parent = nullptr);
    void run() override;

signals:
    void finished();


private:
    FileResult *fileResult;
    QList<FileResult> list;

    double getKeywordNumerator(FileResult *frA, FileResult *frB);
    double getKeywordNorm(FileResult *fr);

    double getLabelNumerator(FileResult *frA, FileResult *frB);
    double getLabelNorm(FileResult *fr);

    double getAttrDegree(FileResult *frA, FileResult *frB);


};

#endif // RELATIONCALCULATETASK_H
