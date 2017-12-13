#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <QStringList>
#include "deps/cppjieba/Jieba.hpp"

const char *const DICT_PATH = "dict/jieba.dict.utf8";
const char *const HMM_PATH = "dict/hmm_model.utf8";
const char *const USER_DICT_PATH = "dict/user.dict.utf8";
const char *const IDF_PATH = "dict/idf.utf8";
const char *const STOP_WORD_PATH = "dict/stop_words.utf8";

class Toolkit {
public:
    Toolkit();
    static QStringList getSegment(QString text);

private:
    cppjieba::Jieba *jieba = new cppjieba::Jieba(DICT_PATH,
                                                 HMM_PATH,
                                                 USER_DICT_PATH,
                                                 IDF_PATH,
                                                 STOP_WORD_PATH);
};

#endif // TOOLKIT_H
