#ifndef TOOLKIT_H
#define TOOLKIT_H

#include <QStringList>
#include "deps/cppjieba/Jieba.hpp"

const char *const DICT_PATH = "C:/Projects/SmartFilesManager/dict/jieba.dict.utf8";
const char *const HMM_PATH = "C:/Projects/SmartFilesManager/dict/hmm_model.utf8";
const char *const USER_DICT_PATH = "C:/Projects/SmartFilesManager/dict/user.dict.utf8";
const char *const IDF_PATH = "C:/Projects/SmartFilesManager/dict/idf.utf8";
const char *const STOP_WORD_PATH = "C:/Projects/SmartFilesManager/dict/stop_words.utf8";

//Singleton class
class Toolkit {

public:
    static Toolkit &getInstance();
    Toolkit(Toolkit const &) = delete;            // Copy construct
    Toolkit(Toolkit &&) = delete;                 // Move construct
    Toolkit &operator=(Toolkit const &) = delete; // Copy assign
    Toolkit &operator=(Toolkit &&) = delete;      // Move assign

    QStringList getSegment(QString text);

private:
    Toolkit();
    cppjieba::Jieba *jieba;

};

#endif // TOOLKIT_H
