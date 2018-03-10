#include "searchbox.h"

SearchBox::SearchBox(const QList<File> *li, QWidget *parent) : QLineEdit(parent)
{
    popup = new SearchPopup(li, this);
    fileList = li;

    connect(this, SIGNAL(returnPressed()), this, SLOT(doSearch()));
    connect(popup, SIGNAL(done()), this, SLOT(doSearchFromSuggest()));
}

void SearchBox::doSearch()
{
    QString str = text();
    int jump = 0;
    if (str.isEmpty())
        return;

    if (lastText != str)
        findNextCount = 0;

    QString firstFile = "";//use for cycle search
    bool allJump = false;
    for (int i = 0; i < fileList->count(); ++i)
    {
        if (fileList->at(i).name.contains(str, Qt::CaseInsensitive))
        {
            if (firstFile.isEmpty())
                firstFile = fileList->at(i).path;
            if (lastText == str && jump < findNextCount)
            {
                jump++;
                continue;
            }
            else
            {
                emit findFile(fileList->at(i).path);
                break;
            }
        }
        if (i == fileList->count() - 1 && !firstFile.isEmpty())
        {
            allJump = true;
        }
    }
    lastText = str;
    if (allJump)
    {
        findNextCount = 1;
        emit findFile(firstFile);
    }
    else
        ++findNextCount;
}

void SearchBox::doSearchFromSuggest()
{
    lastText = "";
    findNextCount = 0;
    if (text().isEmpty())
        return;
    emit findFile(text());
}

