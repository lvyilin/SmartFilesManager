#include "graphwidget.h"
#include <ogdf/energybased/FMMMLayout.h>
using namespace ogdf;
using namespace std;

graphwidget::graphwidget(QWidget *parent, DBHelper *db) :
    QWidget(parent), dbHelper(db)
{
    setMinimumSize(208, 427);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //可选;
    a = new graph_(db);
    qDebug() << "here";
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
    dolayout();
    qDebug() << a->nodelist.count();
    for (int i = 0; i < a->nodelist.count(); i++)
    {

        painter.setBrush(QColor(155, 128, 138, 135));
        painter.setPen(QPen(Qt::black, 5));
        painter.drawEllipse(a->nodelist[i].x, a->nodelist[i].y, 20, 20);
        foreach (Relation r, a->nodelist[i].relations)
        {
            for (int b = 0; b < a->nodelist.count(); b++)
            {
                if (r.file.path == a->nodelist[b].path)
                {
                    painter.setPen(Qt::green);
                    painter.drawLine(a->nodelist[i].x, a->nodelist[i].y, a->nodelist[b].x, a->nodelist[b].y);
                    painter.drawText((a->nodelist[i].x + a->nodelist[b].x) / 2 + 10,
                                     (a->nodelist[i].y + a->nodelist[b].y) / 2 + 10,
                                     QString::number(r.keywordDegree * KEYWORD_RELATION_WEIGHT
                                                     + r.labelDegree * LABEL_RELATION_WEIGHT
                                                     + r.attributeDegree * ATTRIBUTE_RELATION_WEIGHT));
                }
            }
        }
    }
    painter.end();
    qDebug() << "draw graph really finished";
}

void graphwidget::dolayout()
{
    Graph G;
    for (int i = 0; i < a->nodelist.count() ; i++)
    {
        a->nodelist[i].ogdfID = G.newNode(i);
    }
    qDebug() << "hei";
    qDebug() << a->edgelist.count();
    for (int i = 0; i < a->edgelist.count(); i++)
    {
        qDebug() << a->edgelist[i].first->name;
        qDebug() << a->edgelist[i].second->name;
        G.newEdge(a->edgelist[i].first->ogdfID, a->edgelist[i].second->ogdfID);
    }
    GraphAttributes GA_FMMM(G);
    FMMMLayout Layout;
    Layout.useHighLevelOptions(true);
    Layout.unitEdgeLength(15.0);
    Layout.newInitialPlacement(true);
    Layout.qualityVersusSpeed(FMMMLayout::qvsGorgeousAndEfficient);
    Layout.call(GA_FMMM);

    DRect rect = GA_FMMM.boundingBox();
    double xk = NORMALIZED_GRAPH_WIDTH_AND_HEIGHT / rect.width();
    double yk = NORMALIZED_GRAPH_WIDTH_AND_HEIGHT / rect.height();
    double x0 = rect.p1().m_x; //最小x
    double y0 = rect.p1().m_y; //最小y
    qDebug() << a->nodelist.count();
    for (int i = 0; i < a->nodelist.count(); i++)
    {
        //坐标转化到 [0,NORMALIZED_GRAPH_WIDTH_AND_HEIGHT] 区间内
        a->nodelist[i].x = (GA_FMMM.x(a->nodelist[i].ogdfID) - x0) * xk + NORMALIZED_GRAPH_LEFT;
        a->nodelist[i].y = (GA_FMMM.y(a->nodelist[i].ogdfID) - y0) * yk + NORMALIZED_GRAPH_TOP;
    }
}

