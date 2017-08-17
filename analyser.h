#ifndef ANALYSER_H
#define ANALYSER_H

#include <QObject>
#include <QMimeDatabase>
#include <QMimeType>
#include "dbhelper.h"

class Analyser : public QObject {
    Q_OBJECT
public:
    explicit Analyser(DBHelper *dh, QObject *parent = nullptr);

    bool isSupportedFormat(QString format) const;
    QStringList getSupportedFormatsList() const;
    QStringList getSupportedFormatsFilter() const;
    bool processFile(const File &file);
signals:
public slots:
private:
    const QStringList supportedFormat = {"docx", "txt"};
    const QStringList supportedFormatFilter = {"*.docx", "*.txt"};
    DBHelper *dbHelper;
    QMimeDatabase mimeDb;
    QString docxExtract(const File &file);
};

#endif // ANALYSER_H
