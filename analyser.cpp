#include "analyser.h"

Analyser::Analyser(File f, QObject *parent) :
    QObject(parent),
    file(f)
{

}

bool Analyser::isSupportedFormat(QString format)
{
    return supportedFormat.contains(format);
}
