#include "wordlabelwidget.h"
#include "QtMath"

wordlabelwidget::wordlabelwidget(QWidget *parent, DBHelper *db, ConfigHelper *cf) :
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
    db->getFileResults(frs);

}

wordlabelwidget::~wordlabelwidget()
{

}

void wordlabelwidget::setpath(QString filepath_)
{
    filepath = filepath_;
    for (int i = 0; i < frs.count(); i++)
    {
        if (frs[i].file.path == filepath)
        {
            filename = frs[i].file.name;
            labellist = frs[i].labels;
            id = i;
            break;
        }
    }
    is_drawed = false;
    this->update();
}

void wordlabelwidget::paintEvent(QPaintEvent *event)
{
    list->clear();
    files.clear();
    l_file.clear();
    rec = this->geometry(); //获取窗口的位置以及大小并保存在rec中。
    Q_UNUSED(event);
    QPainter painter(this);
    QFont font1;
    font1.setFamily("Microsoft YaHei");
    // 大小
    font1.setPointSize(8);
    font1.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
    option.setWrapMode(QTextOption::WordWrap);
    painter.setFont(font1);
    painter.setBrush(Qt::gray);
    QRect filerec;
    filerec.setX(rec.center().x() - rec.width() * 0.1);
    filerec.setY(rec.center().y() - rec.height() * 0.1);
    filerec.setWidth(rec.width() * 0.15);
    filerec.setHeight(rec.width() * 0.15);
    painter.drawRect(filerec);
    painter.drawText(filerec, filename, option);
    int r = rec.height() * 0.30;
    for (int i = 0; i < labellist.count(); i++)
    {
        painter.drawEllipse(rec.center().x() - rec.width() * 0.1 + rec.width() * 0.075 + r * qCos(360 * (i + 1) / labellist.count()),
                            rec.center().y() - rec.height() * 0.1 + rec.width() * 0.075 + r * qSin(360 * (i + 1) / labellist.count()),
                            rec.width() * 0.05,
                            rec.width() * 0.05);
        painter.drawText(rec.center().x() - rec.width() * 0.1 + rec.width() * 0.075 + r * qCos(360 * (i + 1) / labellist.count()),
                         rec.center().y() - rec.height() * 0.1 + rec.width() * 0.075 + r * qSin(360 * (i + 1) / labellist.count()),
                         labellist[i].name);
    }



    for (int i = 0 ; i < labellist.count(); i++)
    {
        if (qPow(h_Point.x() - (rec.center().x() - rec.width() * 0.1 + rec.width() * 0.075 + r * qCos(360 * (i + 1) / labellist.count())), 2)
                +
                qPow(h_Point.y() - ((rec.center().y() - rec.height() * 0.1) + rec.width() * 0.075 + r * qSin(360 * (i + 1) / labellist.count())), 2)
                <=
                qPow(rec.width() * 0.05, 2)
           )
        {
            dbHelper->getFilesHaveLabel(l_file, labellist[i].name);
            for (int j = 0; j < l_file.count(); j++)
            {
                files.append(l_file[j].name);
            }
            list->addItems(files);
            break;
        }
    }
    painter.end();
    is_drawed = true;
}

void wordlabelwidget::mousePressEvent(QMouseEvent *event)
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


void wordlabelwidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) //如果鼠标按下的是左键
    {
        QPoint tmp;
        tmp = event->globalPos() - offset;
        move(tmp);
        return;
    }
}

void wordlabelwidget::mouseReleaseEvent(QMouseEvent *event)
{
    //拖动完成后，光标恢复默认形状
    setCursor(cursor);
    //或者直接用自带恢复鼠标指针形状的函数为：QApplication::restoreOverrideCursor();
    //但是需要前面设置哪个是默认的光标形状，用这个函数setOverrideCursor()函数
}


void wordlabelwidget::wheelEvent(QWheelEvent *event)
{
    if (event->delta() > 0) //如果滚轮往上滚
    {
        rec.setWidth(rec.width() + 25); //设置宽度为原有基础上+25
        rec.setHeight(rec.height() + 15); //设置窗口高度为原有基础上+20
        this->setGeometry(rec);//然后设置窗口大小。
        this->update();
    }
    else  //同样的
    {
        rec.setWidth(rec.width() - 25); //设置宽度为原有基础上+25
        rec.setHeight(rec.height() - 15); //设置窗口高度为原有基础上+20
        if (this->minimumSize().height() < rec.height() && this->minimumSize().width() < rec.width())
            this->setGeometry(rec);
        this->update();
    }
}

void wordlabelwidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    h_Point = event->pos();
    this->update();
}
