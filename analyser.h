#ifndef ANALYSER_H
#define ANALYSER_H

#include <QObject>
struct File;

class Analyser : public QObject {
    Q_OBJECT
public:
    explicit Analyser(File f, QObject *parent = nullptr);

    bool isSupportedFormat(QString format);

signals:
public slots:
private:
    const QStringList supportedFormat = {"docx", "txt"};
    File file;
};

#endif // ANALYSER_H
