#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif    //解决MSVC编译UTF-8(BOM)导致的中文编码问题

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("智能文件管家");
    QCoreApplication::setOrganizationName("SFM");
    QCoreApplication::setApplicationName("SmartFilesManager");
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
