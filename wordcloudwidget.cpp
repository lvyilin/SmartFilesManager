#include "wordcloudwidget.h"

wordcloudwidget::wordcloudwidget(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(208, 427);
}

wordcloudwidget::~wordcloudwidget()
{
}

//draw wordcloud
void wordcloudwidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QString name = a->filename;
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawEllipse(339, 242, 40, 40);
    painter.drawText(339, 242, name);

    double temp_x, temp_y;
    QString temp_keyword;
    double temp_size;
    int rand = 0;
    QMapIterator<QString, double> map(a->Keywords);
    while (map.hasNext())
    {
        map.next();
        temp_keyword = map.key();
        temp_size = map.value();
        if (rand == 0)
        {
            temp_x = 339 + 20 * temp_size + 10;
            temp_y = 242 + 20 * temp_size + 10;
            rand ++;
        }
        else if (rand % 2 == 0)
        {
            if (339 + 20 * temp_size + 10 == temp_x || 242 + 20 * temp_size + 10 == temp_y)
            {
                temp_x += 20 ;
                temp_y += 20 ;
            }
            else
            {
                temp_x = 339 + 20 * temp_size + 10  ;
                temp_y = 242 + 20 * temp_size + 10 ;
            }
        }
        else
        {
            if (339 + 20 * temp_size + 10 == temp_x || 242 + 20 * temp_size + 10 == temp_y)
            {
                temp_x -= 20 ;
                temp_y -= 20 ;
            }
            else
            {
                temp_x = 339 - 20 * temp_size + 10  ;
                temp_y = 242 - 20 * temp_size + 10 ;
            }
        }
        painter.drawEllipse(temp_x, temp_y, 20 + temp_size * 15, 20 + temp_size * 15);
        painter.drawText(temp_x - 10, temp_y, temp_keyword);
    }
}

void wordcloudwidget::setwordcloud()
{
    a = new wordcloud();
    a->filename = ;

    a->Keywords = ;
}
