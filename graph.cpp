#include"graph.h"

graph::graph(QList<node> nodelist_, QList<edge> edgrlist_)
{
    nodelist = nodelist, edgelist = edgelist;
}

void graph::drawgraph(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(155, 128, 138, 135));
    painter.setPen(QPen(Qt::black, 5));
    foreach (node n, nodelist)
    {
        painter.drawEllipse(n.getcoordinate().first, n.getcoordinate().second, size, size);
    }
    painter.setBrush(QPen(Qt::black, 2));
    foreach (edge e, edgelist)
    {
        painter.drawLine(
            e.getpair().first.getcoordinate().first,
            e.getpair().first.getcoordinate().second,
            e.getpair().second.getcoordinate().first,
            e.getpair().second.getcoordinate().second
        );
    }
}
