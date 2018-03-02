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
    void finished(FileResult *fr);


private:
    FileResult *fileResult;
    QList<FileResult> list;

    double getKeywordNumerator(FileResult *frA, FileResult *frB);
    double getKeywordNorm(FileResult *fr);

    double getLabelNumerator(FileResult *frA, FileResult *frB);
    double getLabelNorm(FileResult *fr);

    double getAttrDegree(FileResult *frA, FileResult *frB);
    bool isSameType(const QString &fmt1, const QString &fmt2);


};

#endif // RELATIONCALCULATETASK_H
