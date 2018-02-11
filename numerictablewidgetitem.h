#ifndef NUMERICTABLEWIDGETITEM_H
#define NUMERICTABLEWIDGETITEM_H

#include <QTableWidgetItem>

class NumericTableWidgetItem: public QTableWidgetItem {
public:
    NumericTableWidgetItem(const QString &text);
    bool operator <(const QTableWidgetItem &other) const;
};

#endif // NUMERICTABLEWIDGETITEM_H
