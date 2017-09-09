#-------------------------------------------------
#
# Project created by QtCreator 2017-07-01T20:32:26
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartFilesManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    settingsdialog.cpp \
    confighelper.cpp \
    dbhelper.cpp \
    analyser.cpp \
    fileupdaterthread.cpp

HEADERS += \
        mainwindow.h \
    settingsdialog.h \
    confighelper.h \
    dbhelper.h \
    analyser.h \
    fileupdaterthread.h

FORMS += \
        mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    images.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/deps/zlib/ -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/deps/zlib/ -lzlibd
else:unix: LIBS += -L$$PWD/deps/zlib/ -lzlib

INCLUDEPATH += $$PWD/deps/zlib/include
DEPENDPATH += $$PWD/deps/zlib/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/deps/quazip/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/deps/quazip/ -lquazipd
else:unix: LIBS += -L$$PWD/deps/quazip/ -lquazip

INCLUDEPATH += $$PWD/deps/quazip/include
DEPENDPATH += $$PWD/deps/quazip/include
