#ifndef WORDCLOUDWINDOW_H
#define WORDCLOUDWINDOW_H

#include <QMainWindow>

namespace Ui {
class WordCloudWindow;
}

class WordCloudWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit WordCloudWindow(QWidget *parent = 0);
    ~WordCloudWindow();

private slots:


private:
    Ui::WordCloudWindow *ui;
};

#endif // WORDCLOUDWINDOW_H
