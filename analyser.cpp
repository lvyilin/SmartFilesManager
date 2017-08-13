#include "analyser.h"

Analyser::Analyser(File f, DBHelper *dh, QObject *parent) :
    QObject(parent),
    dbHelper(dh),
    file(f)
{

}

bool Analyser::isSupportedFormat(QString format)
{
    return supportedFormat.contains(format);
}
