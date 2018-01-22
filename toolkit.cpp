﻿#include "toolkit.h"
#include <vector>
#include <string>
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
    jieba = new cppjieba::Jieba(DICT_PATH,
                                HMM_PATH,
                                USER_DICT_PATH,
                                IDF_PATH,
                                STOP_WORD_PATH);
}
