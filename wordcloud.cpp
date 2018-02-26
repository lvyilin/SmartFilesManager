#include "wordcloud.h"

wordcloud::wordcloud()
{

}

void wordcloud::setwordcloud(QString filename_, QMap<QString/*keyword*/, double/*size*/ > Keywords_)
{
    filename = filename_;
    Keywords = Keywords_;
}

