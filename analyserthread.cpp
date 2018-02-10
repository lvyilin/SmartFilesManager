#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "analyserthread.h"
#include <qDebug>
#include "toolkit.h"

AnalyserThread::AnalyserThread(DBHelper *db, const QList<File> &f, QObject *parent) :
    dbHelper(db),
    fileList(f),
    QThread(parent)
{
    abortFlag = false;
}

void AnalyserThread::run()
{
    int successCount, failCount;
    successCount = failCount = 0;
    foreach (File file, fileList)
    {
        if (abortFlag)
            return;

        ProcessingResult result = processFile(file);
        switch (result)
        {
        case ProcessAborted:
            return;
        case NoException:
            ++successCount;
            dbHelper->setFinished(file, true);
            break;

        case FileNotFoundException:
        case FileAccessException:
        case FileReadException:
        case FileFormatNotSupported:
        case DocxExtractException:
            ++failCount;
            dbHelper->setValid(file, false);
            break;
        default:
            break;
        }

    }
    emit resultReady(successCount, failCount);
}

void AnalyserThread::abortProgress()
{
    abortFlag = true;
    emit aborted();
}

ProcessingResult AnalyserThread::processFile(const File &file)
{
    QFileInfo checkFile(file.path);
    if (!(checkFile.exists() && checkFile.isReadable()))
    {
        qDebug() << "【AnalyserThread】file not exist or unreadable!";
        return FileNotFoundException;
    }

    if (abortFlag) return ProcessAborted;

    QString textContent;
    //纯文本文件
    if (file.format == "txt")
    {
        QFile f(file.path);
        if (!f.open(QFile::ReadOnly | QFile::Text))
            return FileAccessException;

        QTextStream in(&f);
        textContent = in.readAll();//TODO:可优化，如果文件过大
        if (textContent.isNull())
        {
            qDebug() << "【Analyser】extract txt file failed" << file.name;
            return FileReadException;
        }
        f.close();
        qDebug() << "【Analyser】 extract txt file success! " << file.name;
    }
    //DOCX文件
    else if (file.format == "docx")
    {
        textContent = docxExtract(file);
        if (textContent.isNull())
        {
            qDebug() << "【Analyser】extract docx file failed" << file.name;
            return DocxExtractException;
        }
    }
    else return FileFormatNotSupported;

    if (abortFlag) return ProcessAborted;
//
//开始对文件内容进行处理
//
    FileProduct fileProduct;
    fileProduct.file = file;
    fileProduct.contents = textContent;

    generateKeywords(fileProduct);
    if (abortFlag) return ProcessAborted;
    dbHelper->setFileProduct(fileProduct);
    if (abortFlag) return ProcessAborted;
    generateFileLabels(fileProduct);
    if (abortFlag) return ProcessAborted;
    return NoException;
}

void AnalyserThread::generateKeywords(FileProduct &fpd)
{
    //TODO: qmap use pointer

    qDebug() << "start generate keywords, file: "
             << fpd.file.name;
    fpd.keywords = Toolkit::getInstance().getKeywords(fpd.contents);
    if (abortFlag) return;
    QMap<QString, double> filenameMap =
        Toolkit::getInstance().getKeywords(fpd.file.name.split(".", QString::SkipEmptyParts).at(0));
    QMapIterator<QString, double> itr(filenameMap);
    while (!abortFlag && itr.hasNext())
    {
        itr.next();
        double weight = itr.value() * FILENAME_WEIGHTED_VARIANCE;
        if (fpd.keywords.contains(itr.key()))
        {
            fpd.keywords[itr.key()] += weight;
        }
        else
        {
            fpd.keywords[itr.key()] = weight;
        }
    }
    qDebug() << "generate done";
}

void AnalyserThread::generateFileLabels(FileProduct &fpd)
{
    //TODO: 是否需要再切词?
    qDebug() << "start generate labels, file: "
             << fpd.file.name;
    QMapIterator<QString, double> itr(fpd.keywords);
    QStringList keywords;
    while (!abortFlag && itr.hasNext())
    {
        itr.next();
        keywords.append(itr.key());
    }
    if (abortFlag)return;
    dbHelper->setFileLabels(fpd, keywords);
}

QString AnalyserThread::docxExtract(const File &file)
{
    QuaZip zipper(file.path);
    if (!zipper.open(QuaZip::mdUnzip))
    {
        qDebug() << "【Analyser】cannot open docx in zip format:" << file.name;
        zipper.close();
        return QString();
    }
    if (!zipper.setCurrentFile("word/document.xml"))
    {
        qDebug() << "【Analyser】cannot find docx:" << file.name;
        zipper.close();
        return QString();
    }

    if (abortFlag) return QString();

    QuaZipFile searchFile(&zipper);
    if (!searchFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "【Analyser】cannot open document.xml:" << file.name
                 << "error code：" << searchFile.getZipError();
        searchFile.close();
        zipper.close();
        return QString();
    }

    if (abortFlag) return QString();

    QByteArray content = searchFile.readAll();
    qDebug() << "【Analyser】extract docx file success! size:" << file.name << content.size();
    searchFile.close();
    zipper.close();

    if (abortFlag) return QString();

    QString ret;
    QDomDocument xmlReader("mydoc");
    xmlReader.setContent(content, false);
    QDomNodeList qnl = xmlReader.elementsByTagName("w:t");
    for (int i = 0; i < qnl.count(); i++)
    {
        if (abortFlag) return QString();
        ret += qnl.item(i).toElement().text();
    }
    return ret;
}
