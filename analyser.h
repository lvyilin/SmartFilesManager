#ifndef ANALYSER_H
#define ANALYSER_H

#include <QObject>
#include "dbhelper.h"

class Analyser : public QObject {
    Q_OBJECT
public:
    explicit Analyser(File f, DBHelper *dh, QObject *parent = nullptr);

    bool isSupportedFormat(QString format);

signals:
public slots:
private:
    const QStringList supportedFormat = {"docx", "txt"};
    File file;
    DBHelper *dbHelper;
};

#endif // ANALYSER_H
