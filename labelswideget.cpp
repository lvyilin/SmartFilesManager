#include "labelswideget.h"

labelswideget::labelswideget(QWidget *parent, DBHelper *db, ConfigHelper *cf) :
    QWidget(parent), dbHelper(db), configHelper(cf)
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
    dbHelper = db;
}

void labelswideget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i <  ; i++)
    {
        painter.setBrush(QColor(choosecolor(i % 50)));
        painter.setPen(choosecolor(i % 50));
        painter.drawPie(300, 300, 100, 100, 0, 360 * 16 * 1 /);
        painter.end();
    }
}



void labelswideget::mousePressEvent(QMouseEvent *event)
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

void labelswideget::mouseDoubleClickEvent(QMouseEvent *event)
{

    this->update();
}

void labelswideget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) //如果鼠标按下的是左键
    {
        QPoint tmp;
        tmp = event->globalPos() - offset;
        move(tmp);
        return;
    }
}

void labelswideget::mouseReleaseEvent(QMouseEvent *event)
{
    //拖动完成后，光标恢复默认形状
    setCursor(cursor);
    //或者直接用自带恢复鼠标指针形状的函数为：QApplication::restoreOverrideCursor();
    //但是需要前面设置哪个是默认的光标形状，用这个函数setOverrideCursor()函数
}

QString labelswideget::choosecolor(int i)
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
