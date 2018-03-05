#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "graphwidget.h"
#include "qfont.h"
#include "qmath.h"
#include"qtimer.h"
#include <ogdf/energybased/FMMMLayout.h>
using namespace ogdf;
using namespace std;

graphwidget::graphwidget(QWidget *parent, DBHelper *db) :
    QWidget(parent), dbHelper(db)
{
    setMinimumSize(208, 427);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //可选;
    labelName = new QLabel(this);
    labelName->setStyleSheet("QLabel{color: green; font: 13pt bold;}");
    labelInfo = new QLabel(this);
    labelInfo->setStyleSheet("QLabel{color: gray;}");
    verlayout = new QVBoxLayout();
    verlayout->setContentsMargins(0, 0, 0, 0);
    verlayout->addWidget(labelName);
    verlayout->addWidget(labelInfo);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10000);
    dbHelper = db;
}

graphwidget::~graphwidget()
{
}

void graphwidget::setText(QString name, QString info)
{
    labelName->setText(name);
    labelInfo->setText(info);
}


//draw graph
void graphwidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (is_drawed == false)
    {
        a = new graph_(dbHelper);
        a->start();
        dolayout();
        is_drawed = true;
    }
    int z = 1;
    rec = this->geometry(); //获取窗口的位置以及大小并保存在rec中。
    for (int i = 0; i < a->nodelist.count(); i++)
    {
        painter.setBrush(QColor(choosecolor(i % 50)));
        painter.setPen(QPen(QColor(choosecolor(i % 50)), 3));
        for (int j = 0; j < a->edgelist.count(); j++)
        {
            if (a->edgelist[j].first->path == a->nodelist[i].path)
            {
                painter.drawEllipse(a->nodelist[i].x - 8, a->nodelist[i].y - 8, 20, 20);
                painter.drawEllipse(a->edgelist[j].second->x - 8, a->edgelist[j].second->y - 8, 20, 20);
                painter.drawLine(a->nodelist[i].x, a->nodelist[i].y, a->edgelist[j].second->x, a->edgelist[j].second->y);
            }
        }
        z++;
    }
    for (int i = 0; i < a->nodelist.count(); i++)
    {
        //qDebug() << (h_Point.x() - a->nodelist[i].x) * (h_Point.x() - a->nodelist[i].x) + (h_Point.y() - a->nodelist[i].y) * (h_Point.y() - a->nodelist[i].y);
        if ((h_Point.x() - a->nodelist[i].x) * (h_Point.x() - a->nodelist[i].x) + (h_Point.y() - a->nodelist[i].y) * (h_Point.y() - a->nodelist[i].y) <= 300)
        {
            int counter = 0;
            QString edge_inf;
            QString labels, keywords;
            QMap<QString, double>::const_iterator it;
            int temp = 0;
            if (a->nodelist[i].labels.count() > 1)
            {
                foreach (Label l, a->nodelist[i].labels)
                {
                    labels += l.name + l.type + l.level + " ";
                    if (temp == 10)
                    {
                        break;
                    }
                    temp++;
                    qDebug() << labels;
                }
            }
            else
            {
                labels = "NULL";
            }
            temp = 0;
            painter.drawLine(h_Point.x(), h_Point.y(), h_Point.x() + 1, h_Point.y() + 4);
            QFont font;
            font.setFamily("Microsoft YaHei");
            // 大小
            font.setPointSize(8);
            font.setLetterSpacing(QFont::AbsoluteSpacing, 1);
            painter.setFont(font);
            painter.setPen(Qt::black);
            painter.setBrush(QColor("#1FFFFF"));
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
            option.setWrapMode(QTextOption::WordWrap);
            painter.drawText(h_Point.x() + 225, h_Point.y() + 4, "name: " + a->nodelist[i].name + "\n");
            painter.drawText(h_Point.x() + 225, h_Point.y() + 26, + "labels: " + labels + "\n");
            for (it = a->nodelist[i].keywords.constBegin(); it != a->nodelist[i].keywords.constEnd(); ++it)
            {
                keywords = it.key() + "    value:  " + QString::number(it.value());
                painter.drawText(h_Point.x() + 225, h_Point.y() + 48 + temp * 16, "keywords:  " + keywords);
                painter.drawText(h_Point.x() + 225, h_Point.y() + 48 + temp * 16, "keywords:  " + keywords);
                temp++;
            }

            painter.drawEllipse(a->nodelist[i].x - 8, a->nodelist[i].y - 8, 25, 25);

            foreach (edge_ e, a->edgelist)
            {
                if (e.first->path == a->nodelist[i].path)
                {
                    counter += 1;
                    painter.setPen(QPen(QColor("#734488"), 3));
                    painter.drawLine(e.first->x, e.first->y, e.second->x, e.second->y);
                    painter.setPen(Qt::black);
                    painter.drawText(e.second->x,  e.second->y,  QString::number(counter));
                    edge_inf = "This node connect to node " + QString::number(counter) + ": " + e.second->name + "weight :" + QString::number(e.weight);
                    painter.drawText(h_Point.x() + 225, h_Point.y() - 18 * counter, edge_inf);
                }
            }
            /*(h_Point.x() + 220,
             h_Point.y() - 5 - counter * 18,
             500,
             (h_Point.y() + 48 + a->nodelist[i].keywords.count() * 16 + 5) - (h_Point.y() - 5 - counter * 18));*/
            break;
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
    for (int i = 0; i < a->edgelist.count(); i++)
    {
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

void graphwidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    h_Point = event->pos();
    this->update();
}

void graphwidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) //如果鼠标按下的是左键
    {
        QPoint tmp;
        tmp = event->globalPos() - offset;
        move(tmp);
        return;
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
    if (event->delta() > 0) //如果滚轮往上滚
    {
        rec.setWidth(rec.width() + 25); //设置宽度为原有基础上+25
        rec.setHeight(rec.height() + 15); //设置窗口高度为原有基础上+20
        this->setGeometry(rec);//然后设置窗口大小。
        foreach (node_ n, a->nodelist)
        {
            n.x = n.x * 1.1;
            n.y = n.y * 1.1;
        }
    }
    else  //同样的
    {
        rec.setWidth(rec.width() - 25);
        rec.setHeight(rec.height() - 15);
        //如果缩小之后的窗口不小于设置的最小窗口尺寸，则执行缩放。
        if (this->minimumSize().height() < rec.height() && this->minimumSize().width() < rec.width())
            this->setGeometry(rec);
        foreach (node_ n, a->nodelist)
        {
            n.x = n.x * 0.9;
            n.y = n.y * 0.9;
        }
    }
}


QString graphwidget::choosecolor(int i)
{
    switch (i)
    {
    case 0:
        return "#6B0848";
    case 1:
        return "#A40A3C";
    case 2:
        return "#EC610A";
    case 3:
        return "#FFC300";
    case 4:
        return "#0278AE";
    case 5:
        return "#A5ECD7";
    case 6:
        return "#E8FFC1";
    case 7:
        return "#70D4B4";
    case 8:
        return "#FFEBB7";
    case 9:
        return "#BBBBBB";
    case 10:
        return "#EFEFEF";
    case 11:
        return "#E61C5D";
    case 12:
        return "#194769";
    case 13:
        return "#F6F6E9";
    case 14:
        return "#D7EEF2";
    case 15:
        return "#F2855E";
    case 16:
        return "#0E9577";
    case 17:
        return "#970747";
    case 18:
        return "#9F609C";
    case 19:
        return "#351F39";
    case 20:
        return "#36626A";
    case 21:
        return "#CA82F8";
    case 22:
        return "#155263";
    case 23:
        return "#FFB6B9";
    case 24:
        return "#DD3E3E";
    case 25:
        return "#5F4444";
    case 26:
        return "#24BDDF";
    case 27:
        return "#A7FF83";
    case 28:
        return "#071A52";
    case 29:
        return "#7CBD1E";
    case 30:
        return"#EEE685";
    case 31:
        return"#EEE5DE";
    case 32:
        return"#EEDC82";
    case 33:
        return"#EECFA1";
    case 34:
        return"#EEC591";
    case 35:
        return"#EE82EE";
    case 36:
        return"#EE4000";
    case 37:
        return"#EBEBEB";
    case 38:
        return"#E0FFFF";
    case 39:
        return"#DC143C";
    case 40:
        return"#DB7093";
    case 41:
        return"#CD8500";
    case 42:
        return"#C1FFC1";
    case 43:
        return"#C6E2FF";
    case 44:
        return"#CAFF70";
    case 45:
        return"#8B475D";
    case 46:
        return"#8B0A50";
    case 47:
        return"#778899";
    case 48:
        return"#6E8B3D";
    case 49:
        return"#696969";
    default:
        break;
    }
}

