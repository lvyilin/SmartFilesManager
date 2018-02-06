#include "wordcloud.h"

wordcloud::wordcloud()
{

}

void wordcloud::setwordcloud(QString filename_, QMap<QString/*keyword*/, double/*size*/ > Keywords_)
{
    filename = filename_;
    Keywords = Keywords_;
}

void wordcloud::drawwordcloud(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawEllipse(339, 242, 40, 40);
    painter.drawText(339, 242, QStringLiteral(filename));


    double temp_x, temp_y;
    QString temp_keyword;
    double temp_size;
    int rand = 0;
    QMapIterator<QString, double> map(Keywords);
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
        else if (rand % 2 = 0)
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
        painter.drawText(temp_x - 10, temp_y, QStringLiteral(temp_keyword));
    }
}
