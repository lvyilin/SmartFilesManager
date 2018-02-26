#include "graphwidget.h"

graphwidget::graphwidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(208, 427);
}

graphwidget::~graphwidget()
{
}

//setnodecoordinate
void graphwidget::setnodecoordinate()
{
    double center_x = 339, center_y = 242;
    int i = 0, p = 5;//旋转度数
    QList<edge> *edgel;
    edgel = &a->edgelist;
    foreach (edge e, a->edgelist)
    {
        center_x += 10;
        center_y += 10;
        if (i == 0)
        {
            e.getpair().first.setcoordinate(center_x, center_y);
            e.getpair().first.changeis_set();
            e.getpair().first.setcoordinate(center_x + 14, center_y - 14);
            e.getpair().second.changeis_set();
            continue;
        }
        /**
          * x0= (x - rx0)*cos(a) - (y - ry0)*sin(a) + rx0
          *
          * y0= (x - rx0)*sin(a) + (y - ry0)*cos(a) + ry0
         **/
        foreach (edge e_, *edgel)
        {
            if (e_.getpair().first.id == e.getpair().first.id && e_.getpair().second.getis_set() == false)
            {
                e_.getpair().second.setcoordinate(14 * qCos(180 + p) - (-14) * qSin(180 + p) + center_x, 14 * qSin(180 + p) + -14 * qCos(180 + p) + center_y);
                e_.getpair().second.changeis_set();
                p += 5;
            }
            if (e_.getpair().second.id == e.getpair().second.id && e_.getpair().first.getis_set() == false)
            {
                e_.getpair().first.setcoordinate(14 * qCos(p) - (-14) * qSin(p) + center_x, 14 * qSin(p) + -14 * qCos(p) + center_y);
                e_.getpair().first.changeis_set();
                p += 5;
            }
        }
        i += 5;
        p = 5;
    }


}

//draw graph
void graphwidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(155, 128, 138, 135));
    painter.setPen(QPen(Qt::black, 5));
    setnodecoordinate();
    foreach (node n, a->nodelist)
    {
        painter.drawEllipse(n.getcoordinate().first, n.getcoordinate().second, 20, 20);
    }
    painter.setPen(QPen(Qt::black, 2));
    foreach (edge e, a->edgelist)
    {
        painter.drawLine(
            e.getpair().first.getcoordinate().first,
            e.getpair().first.getcoordinate().second,
            e.getpair().second.getcoordinate().first,
            e.getpair().second.getcoordinate().second
        );
    }
}
