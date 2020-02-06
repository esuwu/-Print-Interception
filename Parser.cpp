
#include "Parser.h"
#include <sys/stat.h>
#include <exception>

using std::string;
using std::cout;
using std::endl;
using std::unordered_map;
using std::ofstream;
using std::ifstream;
using std::istreambuf_iterator;

void Parser::StartParse(){
    while(true){
        fileToParse = queue.RetrieveAndDelete();
        if(fileToParse.size() != 0){
            try{
                Parse();
            }
            catch (std::exception &e)
            {
                //std::cout << e.what() << std::endl;
            }
        }
    }
}

Parser::Parser(string _file_to_write, ThreadSafeQueue & _queue) : queue(_queue), pathToCups("/var/spool/cups"), fileToWrite(_file_to_write) {

}

int Parser::Parse() {
    chmod(pathToCups.c_str(), S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);
    chmod(fileToParse.c_str(), S_IRUSR|S_IRGRP|S_IROTH);

    ifstream file(fileToParse);
    if (!file.is_open()){
        throw std::runtime_error("file opening failed");
    }

    string data_from_file ((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    bool was_byte_key_read = false;
    auto it = myMap.begin();
    auto iter_file = data_from_file.begin() + 8; //  Первые 8 байт не интересуют
    int is_begin_tags = 0;
    int value_length = 0;

    for(iter_file; iter_file != data_from_file.end(); iter_file++) {
        if ( *iter_file > 0 and*iter_file < 15 and *iter_file != 3) { // до 15 begin-attribute-group-tag
            is_begin_tags++;
            iter_file++;
        }
        if (*iter_file == 3) { //  3 - end of attribute group
            is_begin_tags--;
            continue;
        }
        if (*iter_file > 15) { // после 15 value tag
            iter_file++;
        }

        value_length = (((int) *iter_file) << 8) + (int) (*(++iter_file));
        iter_file++;
        if (value_length > 0 and !was_byte_key_read) {
                myMap[tmpKey] = tmpValue;
                tmpKey.clear();
                tmpValue.clear();
            for (int i = 0; i < value_length; i++) {
                tmpKey.push_back(*iter_file);
                if(i < value_length - 1){
                    iter_file++;
                }
            }
            was_byte_key_read = true;
        } else {
            was_byte_key_read = false;
            if (value_length == 0 and !tmpValue.empty()) {
                value_length = (((int) *iter_file) << 8) + (int) (*(++iter_file));
                iter_file++;
                tmpValue.push_back(' ');
            }
            for (int i = 0; i < value_length; i++) {
                tmpValue.push_back(*iter_file);
                if(i < value_length - 1){
                    iter_file++;
                }
            }
        }

    }
    myMap[tmpKey] = tmpValue;

    file.close();
    WriteParseDataToFile();

    return 0;
}

void Parser::WriteParseDataToFile(){
    string tmp_string = fileToWrite;
    tmp_string += "/";
    tmp_string += GetNameFile();
    chmod(tmp_string.c_str(), S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);

    ifstream my_file (tmp_string);
    long begin = my_file.tellg();
    my_file.seekg (0, std::ios::end);
    long end = my_file.tellg();
    if((end - begin) > 0) {
        my_file.close();
        return;
    }
    ofstream outfile(tmp_string);
    for (auto it : myMap)
        outfile << it.first << "   " << it.second << endl;
    CopyDFile();
    outfile.close();
    myMap.clear();
    cout << "Data was successfully recorded" << endl;
}

string Parser::GetNameFile(){
    int count = 0;
    std::string NameFile;
    for(auto it : fileToParse){
        if(count == 4){
            NameFile.push_back(it);
        }
        if(it == '/'){
            count++;
        }
    }
    return NameFile;
}

void Parser::CopyDFile(){
    string d_file = GetNameFile();
    d_file[0] = 'd';
    d_file += "-001";

    string path_to_d_file = pathToCups + string("/") + d_file;

    chmod(path_to_d_file.c_str(), S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);
    ifstream file_in(path_to_d_file, std::ios::binary);
    string out_d_file = fileToWrite + string("/") + d_file;
    chmod(out_d_file.c_str(), S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);
    ofstream file_out(out_d_file, std::ios::binary);
    file_out << file_in.rdbuf();
    file_out.close();
    file_in.close();
}