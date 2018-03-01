#include "graphwidget.h"
#include <ogdf/energybased/FMMMLayout.h>
using namespace ogdf;
using namespace std;

graphwidget::graphwidget(QWidget *parent, DBHelper *db) :
    QWidget(parent), dbHelper(db)
{
    setMinimumSize(208, 427);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //可选;
    dbHelper = db;
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
    if (is_drawed == false)
    {
        a = new graph_(dbHelper);
        qDebug() << "here";
        a->start();
        dolayout();
        is_drawed = true;
    }
    qDebug() << a->nodelist.count();
    for (int i = 0; i < a->nodelist.count(); i++)
    {

        painter.setBrush(QColor(155, 128, 138, 135));
        painter.setPen(QPen(Qt::black, 5));
        painter.drawEllipse(a->nodelist[i].x, a->nodelist[i].y, 20, 20);
        foreach (edge_ e, a->edgelist)
        {
            for (int b = 0; b < a->nodelist.count(); b++)
            {
                if (e.first->path == a->nodelist[i].path && e.second->path == a->nodelist[b].path)
                {
                    painter.setPen(Qt::green);
                    painter.drawLine(a->nodelist[i].x, a->nodelist[i].y, a->nodelist[b].x, a->nodelist[b].y);
                    painter.drawText((a->nodelist[i].x + a->nodelist[b].x) / 2,
                                     (a->nodelist[i].y + a->nodelist[b].y) / 2,
                                     QString::number(e.weight));
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

void graphwidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) //如果鼠标按下的是左键
    {
        //则改变鼠标形状为手掌，表示拖动状态。
        QCursor cursor1;//创建光标对象
        cursor1.setShape(Qt::OpenHandCursor);//设置光标形态
        setCursor(cursor1);//使用手掌光标

        //这里获取指针位置和窗口位置的差值
        offset = event->globalPos() - this->pos();

    }
}
void graphwidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) //如果鼠标按下的是左键
    {
        QPoint tmp;
        tmp = event->globalPos() - offset;
        move(tmp);
    }
}

void graphwidget::mouseReleaseEvent(QMouseEvent *event)
{
    //拖动完成后，光标恢复默认形状
    setCursor(cursor);
    //或者直接用自带恢复鼠标指针形状的函数为：QApplication::restoreOverrideCursor();
    //但是需要前面设置哪个是默认的光标形状，用这个函数setOverrideCursor()函数
}


void graphwidget::wheelEvent(QWheelEvent *event)
{
    /*QRect tmp = this->geometry(); //获取窗口的位置以及大小并保存在tmp中。
    int n =event->delta() / 60;
    double k = 1.0;
    if (event->delta() > 0) //如果滚轮往上滚
    {
        tmp.setWidth(tmp.width() + 25); //设置宽度为原有基础上+25
        tmp.setHeight(tmp.height() + 15); //设置窗口高度为原有基础上+20
        this->setGeometry(tmp);//然后设置窗口大小。
            for(int j=0;j<n;j++)
                k*=1.1;

    }
    else  //同样的
    {
        tmp.setWidth(tmp.width() - 25);
        tmp.setHeight(tmp.height() - 15);
        //如果缩小之后的窗口不小于设置的最小窗口尺寸，则执行缩放。
        if (this->minimumSize().height() < tmp.height() && this->minimumSize().width() < tmp.width())
            this->setGeometry(tmp);
        for(int j=0;j>n;j--)
            k*=0.9;
    }
    viewCenterX += (1-k)*mouseX/viewScale;
    viewCenterY += (1-k)*mouseY/viewScale;*/
}
