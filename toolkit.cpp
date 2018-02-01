#include "toolkit.h"
#include <vector>
#include <string>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

Toolkit &Toolkit::getInstance()
{
    static Toolkit instance;
    return instance;
}

QStringList Toolkit::getSegment(QString text)
{
    std::string s = text.toStdString();
    std::vector<std::string> words;
    jieba->Cut(s, words, true);
    qDebug() << QString::fromStdString(
                 limonp::Join(words.begin(), words.end(), "/"));

    QStringList ret;
    foreach (auto stdStr, words)
    {
        ret.append(QString::fromStdString(stdStr));
    }
    return ret;
}

QMap<QString, double> Toolkit::getKeywords(QString text)
{
    std::string s = text.toStdString();
    std::vector<cppjieba::KeywordExtractor::Word> keywordres;
    jieba->extractor.Extract(s, keywordres, NEEDED_TOP_KEYWORDS);

    QMap<QString, double> res;
    foreach (cppjieba::KeywordExtractor::Word kw, keywordres)
    {
        res.insert(QString::fromStdString(kw.word), kw.weight);
    }
    return res;
}

Toolkit::Toolkit()
{
    const QString DICT_PATH = "/dict/jieba.dict.utf8";
    const QString HMM_PATH = "/dict/hmm_model.utf8";
    const QString USER_DICT_PATH = "/dict/user.dict.utf8";
    const QString IDF_PATH = "/dict/idf.utf8";
    const QString STOP_WORD_PATH = "/dict/stop_words.utf8";

    QString curPath = QDir::currentPath();
    QString dictPath = curPath + DICT_PATH;
    QString prefix;
    if (QFileInfo(dictPath).exists())
        prefix = curPath;
    else
    {
        QDir dir = curPath;
        dir.cd("../SmartFilesManager/");
        if (dir.exists() && QFileInfo(dir.absolutePath() + DICT_PATH).exists())
            prefix = dir.absolutePath();
        else
            qDebug() << "cannot find toolkit dict!";
    }
    jieba = new cppjieba::Jieba(QString(prefix + DICT_PATH).toStdString(),
                                QString(prefix + HMM_PATH).toStdString(),
                                QString(prefix + USER_DICT_PATH).toStdString(),
                                QString(prefix + IDF_PATH).toStdString(),
                                QString(prefix + STOP_WORD_PATH).toStdString()
                               );
}
