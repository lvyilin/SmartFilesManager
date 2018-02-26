#include "searchpopup.h"

SearchPopup::SearchPopup(const QList<File> *li, QLineEdit *parent) : QObject(parent), editor(parent)
{
    fileList = li;
    popup = new QTreeWidget;
    popup->setWindowFlags(Qt::Popup);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->setFocusProxy(parent);
    popup->setMouseTracking(true);

    popup->setColumnCount(1);
    popup->setUniformRowHeights(true);
    popup->setRootIsDecorated(false);
    popup->setEditTriggers(QTreeWidget::NoEditTriggers);
    popup->setSelectionBehavior(QTreeWidget::SelectRows);
    popup->setFrameStyle(QFrame::Box | QFrame::Plain);
    popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    popup->header()->hide();

    popup->installEventFilter(this);

    connect(popup, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
            SLOT(doneSearch()));

    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(autoSuggest()));
    connect(editor, SIGNAL(textEdited(QString)), timer, SLOT(start()));

}

SearchPopup::~SearchPopup()
{
    delete popup;
}

bool SearchPopup::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj != popup)
        return false;

    if (ev->type() == QEvent::MouseButtonPress)
    {
        popup->hide();
        editor->setFocus();
        return true;
    }

    if (ev->type() == QEvent::KeyPress)
    {

        bool consumed = false;
        int key = static_cast<QKeyEvent *>(ev)->key();
        switch (key)
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            doneSearch();
            consumed = true;
            break;

        case Qt::Key_Escape:
            editor->setFocus();
            popup->hide();
            consumed = true;
            break;

        case Qt::Key_Up:
        case Qt::Key_Down:
        case Qt::Key_Home:
        case Qt::Key_End:
        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            break;

        default:
            editor->setFocus();
            editor->event(ev);
            popup->hide();
            break;
        }

        return consumed;
    }

    return false;
}

void SearchPopup::showSearchResult(const QList<File> &choices)
{
    if (choices.isEmpty())
        return;

    const QPalette &pal = editor->palette();
    QColor color = pal.color(QPalette::Normal, QPalette::WindowText);

    popup->setUpdatesEnabled(false);
    popup->clear();
    popup->setFixedWidth(editor->width());
    for (int i = 0; i < choices.count(); ++i)
    {
        QTreeWidgetItem *item;
        item = new QTreeWidgetItem(popup);
        item->setText(0, choices[i].name);
        item->setToolTip(0, choices[i].path);
        item->setTextColor(0, color);
    }
    //    popup->setCurrentItem(popup->topLevelItem(0));
    popup->resizeColumnToContents(0);
    popup->setUpdatesEnabled(true);

    popup->move(editor->mapToGlobal(QPoint(0, editor->height())));
    popup->setFocus();
    popup->show();
}

void SearchPopup::doneSearch()
{
    timer->stop();
    popup->hide();
    editor->setFocus();
    QTreeWidgetItem *item = popup->currentItem();
    if (item)
    {
        editor->setText(item->toolTip(0));
        //        QMetaObject::invokeMethod(editor, "returnPressed");
        emit done();
    }
}

void SearchPopup::preventSuggest()
{
    timer->stop();
}

void SearchPopup::autoSuggest()
{
    QString str = editor->text();
    QList<File> choices;
    for (int i = 0; i < fileList->size(); ++i)
    {
        if (fileList->at(i).name.contains(str, Qt::CaseInsensitive))
            choices << fileList->at(i);
    }
    showSearchResult(choices);
}


