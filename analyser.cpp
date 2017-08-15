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

bool Analyser::isSupportedFormat(QString format)
{
    //支持所有纯文本文件
    if (mimeDb.mimeTypeForFile("*." + format).inherits("text/plain"))
        return true;
    else return supportedFormat.contains(format);
}

QStringList Analyser::getSupportedFormatsList()
{
    return supportedFormatFilter;
}

void Analyser::processFile(File &file)
{
    if (!isSupportedFormat(file.format))
    {
        qDebug() << "[Analyser] not supported file format";
        return;
    }
    QMimeType mime = mimeDb.mimeTypeForFile(file.name);
    if (!mime.isValid())
    {
        qDebug() << "【Analyser】process file is not a valid format: " << file.name;
        return;
    }
    //纯文本文件
    if (mime.inherits("text/plain"))
    {
    }
    //DOCX文件
    else if (file.format == "docx")
    {
        //        QFile docFile(file.path);
        //        if (!docFile.open(QIODevice::ReadOnly))
        //        {
        //            qDebug() << "file open false";
        //            return;
        //        }
        //        QuaZip zipper(&docFile);
        QuaZip zipper(file.path);
        if (!zipper.open(QuaZip::mdUnzip))
        {
            qDebug() << "【Analyser】cannot open docx in zip format:" << file.name;
            zipper.close();
            return;
        }
        if (!zipper.setCurrentFile("word/document.xml"))
        {
            qDebug() << "【Analyser】cannot find docx:" << file.name;
            zipper.close();
            return;
        }
        QuaZipFile searchFile(&zipper);
        if (!searchFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "【Analyser】cannot open document.xml:" << file.name
                     << "error code：" << searchFile.getZipError();
            searchFile.close();
            zipper.close();
            return;
        }
        QByteArray content = searchFile.readAll();
        qDebug() << "【Analyser】extract docx file success! size:" << file.name << content.size();
        searchFile.close();
        zipper.close();

        //-------TODO:save to db----
        QFile saveFile(file.name + ".txt");
        saveFile.open(QIODevice::WriteOnly);
        QTextStream out(&saveFile);

        QDomDocument xmlReader("mydoc");
        xmlReader.setContent(content, false);
        QDomNodeList qnl = xmlReader.elementsByTagName("w:t");
        for (int i = 0; i < qnl.count(); i++)
        {
            out << qnl.item(i).toElement().text();
        }
        saveFile.close();
        //--------------------------
    }
}
