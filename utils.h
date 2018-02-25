#ifndef UTILS_H
#define UTILS_H

#include <QDateTime>
#include <QMap>

struct File
{
    QString name;
    QString format;
    QString path;
    qint64 size;
    QDateTime createTime;
    QDateTime modifyTime;
    bool isFinished;
    bool isValid;
};

struct Label
{
    QString name;
    int level;
    QString type;
    QString parentName;
};

struct Relation
{
    File file;
    double keywordDegree;
    double labelDegree;
    double attributeDegree;
};

struct FileProduct
{
    File file;
    QString contents;
    QMap<QString, double> keywords;
};

struct FileResult
{
    File file;
    QMap<QString, double> keywords;
    QList<Label> labels;
    QList<Relation> relations;
};

enum RunningStrategy
{
    CpuTrigger, TimeTrigger
};

enum InterruptionType
{
    NoInterrupt, AnalyserInterrupt, FileUpdaterInterrupt, DBInterrupt, TerminateInterrupt
};

enum ProcessingResult
{
    NoException,
    ProcessAborted,
    FileNotFoundException,
    FileAccessException,
    FileReadException,
    FileFormatNotSupported,
    DocxExtractException
};

//format
const static QStringList SUPPORTED_FORMATS = {"docx", "txt"};
const static QStringList FORMATS_FILTER = {"*.*"};

const static QStringList DOCUMENT_FORMATS = {"doc", "docx", "xls", "xlsx", "ppt", "pptx", "odt", "rtf",
                                             "txt", "pdf", "htm", "html", "tex"
                                            };

const static QStringList AUDIO_FORMATS = {"3gp",  "aa", "aac", "aax", "act", "aiff", "amr", "ape", "au", "awb",
                                          "dct", "dss", "dvf", "flac", "gsm", "iklax", "ivs", "m4a", "m4b", "m4p",
                                          "mmf", "mp3", "mpc", "msv", "ogg", "oga", "mogg", "opus", "ra", "raw",
                                          "tta", "vox", "wav", "wma", "wv", "8svx"
                                         };

const static QStringList VIDEO_FORMATS = {"mkv", "flv", "vob", "drc", "gifv", "mng", "avi", "mov",
                                          "wmv", "yuv", "rm", "rmvb", "asf", "amv", "mp4", "m4v", "mpg", "mp2",
                                          "mpeg", "mpe", "mpv", "m2v", "svi", "mxf", "roq", "nsv"
                                         };

const static QStringList IMAGE_FORMATS = {"ani", "bmp", "cal", "fax", "gif", "img", "jbg", "jpe", "jpeg", "jpg",
                                          "mac", "pbm", "pcd", "pcx", "pct", "pgm", "png", "ppm", "psd", "ras",
                                          "tga", "tiff", "wmf"
                                         };

//magic value
//Toolkit
const static int NEEDED_TOP_KEYWORDS = 20;
const static double FILENAME_WEIGHTED_VARIANCE = 2.0;
//MainWindow
const static int WORKLIST_SIZE = 100;
const static int MAX_WORKLIST_NUM = 10;
//Analyser
const static int MAX_THREAD_NUM = MAX_WORKLIST_NUM;
//FileUpdater
const static int MAX_FILES_NUMBER = 5000;

//label judge
const static int LABEL_JUDGEMENT_NEEDED_KEYWORD_NUMBER = 2;
//relation calculate
const static double KEYWORD_RELATION_WEIGHT = 0.35;
const static double LABEL_RELATION_WEIGHT = 0.45;
const static double ATTRIBUTE_RELATION_WEIGHT = 0.2;

#endif // UTILS_H
