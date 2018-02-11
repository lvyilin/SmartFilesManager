#include "relationcalculatetask.h"
#include "cmath"
#include <QDir>
#include <QDebug>

RelationCalculateTask::RelationCalculateTask(FileResult *fr, QList<FileResult> li, QObject *parent):
    QObject(parent)
{
    fileResult = fr;
    list = li;
}

void RelationCalculateTask::run()
{
    double keywordNorm = getKeywordNorm(fileResult);
    double labelNorm = getLabelNorm(fileResult);
    for (int i = 0; i < list.size(); ++i)
    {
        FileResult *anotherFileResult = &(list[i]);
        if (fileResult->file.format != anotherFileResult->file.format)
        {
            Relation foo = {anotherFileResult->file, 0.0, 0.0, getAttrDegree(fileResult, anotherFileResult)};
            fileResult->relations << foo;
            continue;
        }

        Relation r;
        r.file = anotherFileResult->file;
        //keyword relation
        double keywordNumerator = getKeywordNumerator(fileResult, anotherFileResult);
        double keywordNormAnother = getKeywordNorm(anotherFileResult);
        double denominator = keywordNorm * keywordNormAnother;
        r.keywordDegree = keywordNumerator / denominator;
        //label relation
        double labelNumerator = getLabelNumerator(fileResult, anotherFileResult);
        double labelNormAnother = getLabelNorm(anotherFileResult);
        r.labelDegree = labelNumerator / (labelNorm * labelNormAnother);
        //attribute relation
        r.attributeDegree = getAttrDegree(fileResult, anotherFileResult);

        fileResult->relations << r;
    }

    emit finished(fileResult);
}

double RelationCalculateTask::getKeywordNumerator(FileResult *frA, FileResult *frB)
{
    double sum = 0.0;
    QMapIterator<QString, double> itr(frA->keywords);
    while (itr.hasNext())
    {
        itr.next();
        sum += itr.value() * frB->keywords.value(itr.key(), 0.0);
    }
    return sum;
}

double RelationCalculateTask::getKeywordNorm(FileResult *fr)
{
    double sum = 0.0;
    QMapIterator<QString, double> itr(fr->keywords);
    while (itr.hasNext())
    {
        itr.next();
        sum += itr.value() * itr.value();
    }
    return sqrt(sum);
}

double RelationCalculateTask::getLabelNumerator(FileResult *frA, FileResult *frB)
{
    int sum = 0.0;
    QMap<QString, int> mapA, mapB;
    for (Label la : frA->labels)
        mapA.insert(la.name, la.type == "field" ? la.level : 1);
    for (Label lb : frB->labels)
        mapB.insert(lb.name, lb.type == "field" ? lb.level : 1);
    QMapIterator<QString, int> itr(mapA);
    while (itr.hasNext())
    {
        itr.next();
        sum += itr.value() * mapB.value(itr.key(), 0.0);
    }
    return (double)sum;
}

double RelationCalculateTask::getLabelNorm(FileResult *fr)
{
    double sum = 0.0;
    for (Label l : fr->labels)
    {
        if (l.type == "field")
            sum += 1.0 * l.level * l.level;
        else
            sum += 1.0;
    }
    return sqrt(sum);
}

double RelationCalculateTask::getAttrDegree(FileResult *frA, FileResult *frB)
{
    double sum = 0.0;
    if (frA->file.format == frB->file.format)
        sum += 0.4;
    else if ((DOCUMENT_FORMATS.contains(frA->file.format) && DOCUMENT_FORMATS.contains(frB->file.format))
             || (IMAGE_FORMATS.contains(frA->file.format) && IMAGE_FORMATS.contains(frB->file.format))
             || (VIDEO_FORMATS.contains(frA->file.format) && VIDEO_FORMATS.contains(frB->file.format))
             || (AUDIO_FORMATS.contains(frA->file.format) && AUDIO_FORMATS.contains(frB->file.format)))
        sum += 0.2;
    else;

    QDir dirAParent(QFileInfo(frA->file.path).dir());
    QDir dirBParent(QFileInfo(frB->file.path).dir());
    QDir dirA = dirAParent;
    QDir dirB = dirBParent;
    dirAParent.cdUp();
    dirBParent.cdUp();
    QString dirAParentStr = dirAParent.absolutePath();
    QString dirBParentStr = dirBParent.absolutePath();
    QString dirAStr = dirA.absolutePath();
    QString dirBStr = dirB.absolutePath();
    if (dirAStr == dirBStr)
        sum += 0.3;
    else if (dirAParentStr == dirBParentStr || dirAParentStr == dirBStr || dirAStr == dirBParentStr)
        sum += 0.2;
    else;

    qint64 dSize = abs(frA->file.size - frB->file.size);
    if (dSize <= 1024 * 1024)
        sum += 0.1;
    else if (dSize <= 8 * 1024 * 1024)
        sum += 0.05;
    else;

    qint64 dDate = abs(frA->file.createTime.daysTo(frB->file.createTime));
    if (dDate <= 1)
        sum += 0.1;
    else;

    dDate = abs(frA->file.modifyTime.daysTo(frB->file.modifyTime));
    if (dDate <= 1)
        sum += 0.1;
    else;

    return sum;
}


