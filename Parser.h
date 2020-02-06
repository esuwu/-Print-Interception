
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "ThreadSaveQueue.h"

class Parser{
public:
    Parser(std::string _file_to_write, ThreadSafeQueue & _queue);
    ~Parser() = default;
    void StartParse();

private:
    int Parse();
    std::string GetNameFile();
    void WriteParseDataToFile();
    std::string fileToWrite;
    std::string fileToParse;
    std::unordered_map<std::string, std::string> myMap;
    std::string tmpKey;
    std::string tmpValue;
    std::string pathToCups;
    ThreadSafeQueue & queue;
    void CopyDFile();

};


#endif //PARSER_H
