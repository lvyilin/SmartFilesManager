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
    settingsdialog.cpp \
    confighelper.cpp \
    dbhelper.cpp \
    analyser.cpp \
    fileupdaterthread.cpp \
    analyserthread.cpp \
    analyser.cpp \
    toolkit.cpp \
    mainwindow.cpp \
    toolkitinitthread.cpp

HEADERS += \
    settingsdialog.h \
    confighelper.h \
    dbhelper.h \
    analyser.h \
    fileupdaterthread.h \
    analyserthread.h \
    utils.h \
    deps/cppjieba/DictTrie.hpp \
    deps/cppjieba/FullSegment.hpp \
    deps/cppjieba/HMMModel.hpp \
    deps/cppjieba/HMMSegment.hpp \
    deps/cppjieba/Jieba.hpp \
    deps/cppjieba/KeywordExtractor.hpp \
    deps/cppjieba/MixSegment.hpp \
    deps/cppjieba/MPSegment.hpp \
    deps/cppjieba/PosTagger.hpp \
    deps/cppjieba/PreFilter.hpp \
    deps/cppjieba/QuerySegment.hpp \
    deps/cppjieba/SegmentBase.hpp \
    deps/cppjieba/SegmentTagged.hpp \
    deps/cppjieba/TextRankExtractor.hpp \
    deps/cppjieba/Trie.hpp \
    deps/cppjieba/Unicode.hpp \
    deps/cppjieba/limonp/ArgvContext.hpp \
    deps/cppjieba/limonp/BlockingQueue.hpp \
    deps/cppjieba/limonp/BoundedBlockingQueue.hpp \
    deps/cppjieba/limonp/BoundedQueue.hpp \
    deps/cppjieba/limonp/Closure.hpp \
    deps/cppjieba/limonp/Colors.hpp \
    deps/cppjieba/limonp/Condition.hpp \
    deps/cppjieba/limonp/Config.hpp \
    deps/cppjieba/limonp/FileLock.hpp \
    deps/cppjieba/limonp/ForcePublic.hpp \
    deps/cppjieba/limonp/LocalVector.hpp \
    deps/cppjieba/limonp/Logging.hpp \
    deps/cppjieba/limonp/Md5.hpp \
    deps/cppjieba/limonp/MutexLock.hpp \
    deps/cppjieba/limonp/NonCopyable.hpp \
    deps/cppjieba/limonp/StdExtension.hpp \
    deps/cppjieba/limonp/StringUtil.hpp \
    deps/cppjieba/limonp/Thread.hpp \
    deps/cppjieba/limonp/ThreadPool.hpp \
    toolkit.h \
    mainwindow.h \
    toolkitinitthread.h

FORMS += \
    settingsdialog.ui \
    mainwindow.ui

RESOURCES += \
    images.qrc


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/deps/zlib/ -lzlib
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/deps/zlib/ -lzlibd
win32 {
    contains(DEFINES, WIN64) {
        CONFIG(release, debug|release){
            LIBS += -L$$PWD/deps/zlib/x64/ -lzlib
        } else {
            LIBS += -L$$PWD/deps/zlib/x64/ -lzlibd
        }
    } else {
        CONFIG(release, debug|release){
            LIBS += -L$$PWD/deps/zlib/ -lzlib
        } else {
            LIBS += -L$$PWD/deps/zlib/ -lzlibd
        }
    }
}


INCLUDEPATH += $$PWD/deps/zlib/include
DEPENDPATH += $$PWD/deps/zlib/include

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/deps/quazip/ -lquazip
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/deps/quazip/ -lquazipd
win32 {
    contains(DEFINES, WIN64) {
        CONFIG(release, debug|release){
            LIBS += -L$$PWD/deps/quazip/x64/ -lquazip
        } else {
            LIBS += -L$$PWD/deps/quazip/x64/ -lquazipd
        }
    } else {
        CONFIG(release, debug|release){
            LIBS += -L$$PWD/deps/quazip/ -lquazip
        } else {
            LIBS += -L$$PWD/deps/quazip/ -lquazipd
        }
    }
}

INCLUDEPATH += $$PWD/deps/quazip/include
DEPENDPATH += $$PWD/deps/quazip/include

DISTFILES +=
