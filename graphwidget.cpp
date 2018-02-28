#include "graphwidget.h"

graphwidget::graphwidget(QWidget *parent, DBHelper *db) :
    QWidget(parent), dbHelper(db)
{
    setMinimumSize(208, 427);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //可选;
    a = new graph(db);
    a->start();
}

graphwidget::~graphwidget()
{
}

//draw graph
void graphwidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QColor(155, 128, 138, 135));
    painter.setPen(QPen(Qt::black, 5));
    qDebug() << a->nodelist.count();
    double center_x = 700, center_y = 400, r = 0, flag = 0;
    int p = 0; //旋转度数
    for (int i = 0 ; i < a->nodelist.count(); i++)
    {
        a->nodelist[i].is_seted = false;
    }
    qDebug() << a->nodelist.count();
    for (int i = 0; i < a->nodelist.count(); i++)
    {
        if (a->nodelist[i].is_seted == false)
        {
            qDebug() << flag++;
            center_x = center_x + 30;
            center_y = center_y + 30;
            a->nodelist[i].x = center_x;
            a->nodelist[i].y = center_y;
            painter.setPen(QPen(Qt::black, 10));
            painter.drawEllipse(a->nodelist[i].x, a->nodelist[i].y, 30, 30);
            a->nodelist[i].is_seted = true;
            qDebug() << a->nodelist[i].x;
            qDebug() << a->nodelist[i].y;
        }
        /**
         * x0= (x - rx0)*cos(a) - (y - ry0)*sin(a) + rx0
         *
         * y0= (x - rx0)*sin(a) + (y - ry0)*cos(a) + ry0
         **/

        foreach (Relation re, a->nodelist[i].relations)
        {
            p = rand() % 15 + 5;
            r = 400 * (re.keywordDegree * KEYWORD_RELATION_WEIGHT
                       + re.labelDegree * LABEL_RELATION_WEIGHT
                       + re.attributeDegree * ATTRIBUTE_RELATION_WEIGHT);
            for (int b = 0; b < a->nodelist.count(); b++)
            {
                if (re.file.path == a->nodelist[b].path && a->nodelist[b].is_seted == false)
                {
                    if (rand() % 2 == 0)
                    {
                        a->nodelist[b].x = r * qCos(180 + p) - (-r) * qSin(180 + p) + center_x;
                        a->nodelist[b].y = r * qSin(180 + p) + -r * qCos(180 + p) + center_y;
                        a->nodelist[b].is_seted = true;
                        painter.setPen(Qt::black);
                        painter.drawEllipse(a->nodelist[b].x, a->nodelist[b].y, 30, 30);
                        painter.setPen(Qt::green);                        //qDebug() << "1234";
                        break;
                    }
                    else
                    {
                        a->nodelist[b].x = r * qCos(p) - (-r) * qSin(p) + center_x;
                        a->nodelist[b].y = r * qSin(p) + -r * qCos(p) + center_y;
                        a->nodelist[b].is_seted = true;
                        painter.setPen(Qt::black);
                        painter.drawEllipse(a->nodelist[b].x, a->nodelist[b].y, 30, 30);
                        painter.setPen(Qt::green);
                        //qDebug() << "12345";
                        break;
                    }
                }
                if (r >= 80)
                    painter.drawLine(a->nodelist[i].x, a->nodelist[i].y, a->nodelist[b].x, a->nodelist[b].y);
            }
        }
    }
    painter.end();
    qDebug() << "draw graph really finished";
}
