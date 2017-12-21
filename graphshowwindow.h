#ifndef GRAPHSHOWWINDOW_H
#define GRAPHSHOWWINDOW_H

#include <QMainWindow>

namespace Ui {
class GraphShowWindow;
}

class GraphShowWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphShowWindow(QWidget *parent = 0);
    ~GraphShowWindow();

private:
    Ui::GraphShowWindow *ui;
};

#endif // GRAPHSHOWWINDOW_H
