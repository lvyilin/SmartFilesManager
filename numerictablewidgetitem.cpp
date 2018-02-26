#include "numerictablewidgetitem.h"

NumericTableWidgetItem::NumericTableWidgetItem(const QString &text):
    QTableWidgetItem(text)
{
}

bool NumericTableWidgetItem::operator <(const QTableWidgetItem &other) const
{
    QString str1 = text();
    QString str2 = other.text();
    str1.chop(1);
    str2.chop(1);
    return str1.toDouble() < str2.toDouble();
}
