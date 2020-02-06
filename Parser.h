
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "ThreadSaveQueue.h"

class Parser{
public:
    Parser(threadSafe_queue & _queue);
    ~Parser() = default;
    void StartParse();

private:
    int Parse();
    std::string GetNameFile();
    void WriteParseDataToFile();
    std::string FileToWrite;
    std::string FileToParse;
    std::unordered_map<std::string, std::string> my_map;
    std::string TmpKey;
    std::string TmpValue;
    threadSafe_queue & queue;
    void CopyDFile();

};


#endif //PARSER_H
