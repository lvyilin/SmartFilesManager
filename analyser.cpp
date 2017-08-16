#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "analyser.h"
#include <QFile>
#include <qDebug>
#include <QDomDocument>
#include "JlCompress.h"
Analyser::Analyser(DBHelper *dh, QObject *parent) :
    QObject(parent),
    dbHelper(dh)
{

}

bool Analyser::isSupportedFormat(QString format) const
{
    //支持所有纯文本文件
    if (mimeDb.mimeTypeForFile("*." + format).inherits("text/plain"))
        return true;
    else return supportedFormat.contains(format);
}

QStringList Analyser::getSupportedFormatsList() const
{
    return supportedFormat;
}

QStringList Analyser::getSupportedFormatsFilter() const
{
    return supportedFormatFilter;
}

bool Analyser::processFile(const File &file)
{
    QFileInfo checkFile(file.path);
    if (!(checkFile.exists() && checkFile.isReadable()))
    {
        qDebug() << "【Analyser】file not exist or unreadable!";
        return false;
    }
    if (!isSupportedFormat(file.format))
    {
        qDebug() << "【Analyser】not supported file format";
        return false;
    }
    QMimeType mime = mimeDb.mimeTypeForFile(file.name);
    if (!mime.isValid())
    {
        qDebug() << "【Analyser】process file is not a valid format: " << file.name;
        return false;
    }

    //文本文件提取文本
    QString textContent;
    //纯文本文件
    if (mime.inherits("text/plain"))
    {
        QFile f(file.path);
        if (!f.open(QFile::ReadOnly | QFile::Text))
            return false;
        QTextStream in(&f);
        textContent = in.readAll();
        if (textContent.isNull())
        {
            qDebug() << "【Analyser】extract file failed" << file.name;
            return false;
        }
    }
    //DOCX文件
    else if (file.format == "docx")
    {
        textContent = docxExtract(file);
        if (textContent.isNull())
        {
            qDebug() << "【Analyser】extract file failed" << file.name;
            return false;
        }
    }
    else;
    /*QFile saveFile(file.name + ".txt");
    saveFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&saveFile);
    out << textContent;
    saveFile.close();*/
    return true;
}

QString Analyser::docxExtract(const File &file)
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
    QuaZipFile searchFile(&zipper);
    if (!searchFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "【Analyser】cannot open document.xml:" << file.name
                 << "error code：" << searchFile.getZipError();
        searchFile.close();
        zipper.close();
        return QString();
    }
    QByteArray content = searchFile.readAll();
    qDebug() << "【Analyser】extract docx file success! size:" << file.name << content.size();
    searchFile.close();
    zipper.close();

    QString ret;
    QDomDocument xmlReader("mydoc");
    xmlReader.setContent(content, false);
    QDomNodeList qnl = xmlReader.elementsByTagName("w:t");
    for (int i = 0; i < qnl.count(); i++)
    {
        ret += qnl.item(i).toElement().text();
    }
    return ret;
}
