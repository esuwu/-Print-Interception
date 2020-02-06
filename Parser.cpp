
#include "Parser.h"
#include <sys/stat.h>

#define FILE_TO_WRITE "/var/spool/DataFromPrinters"
#define PATH_TO_CUPS "/var/spool/cups"

using std::string;
using std::cout;
using std::endl;
using std::unordered_map;
using std::ofstream;
using std::ifstream;
using std::istreambuf_iterator;

void Parser::StartParse(){
    while(true){
        FileToParse = queue.retrieve_and_delete();
        if(FileToParse.size() != 0){
            Parse();
        }
    }
}

Parser::Parser(threadSafe_queue & _queue) : queue(_queue), FileToWrite(FILE_TO_WRITE) {

}

int Parser::Parse() {
    chmod(PATH_TO_CUPS, S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);
    chmod(FileToParse.c_str(), S_IRUSR|S_IRGRP|S_IROTH);

    ifstream file(FileToParse);
    if (!file.is_open()){
//        cout << "File was not opened" << endl;
//        return -1;
    }
    string data_from_file ((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    bool was_byte_0x00_read = false;
    int count_of_next_symbols = 0;
    bool was_word_read = false;
    auto it = my_map.begin();
    /*Первые 7 байт не интересуют*/
    int useless_bytes = 7;

    for(auto it : data_from_file){
        if(useless_bytes > 0){
            useless_bytes--;
            continue;
        }
        if((int)it == 0){
            was_byte_0x00_read = true;
            continue;
        }
        if(was_byte_0x00_read == true){
            count_of_next_symbols = (int)it;
            was_byte_0x00_read = false;
            continue;
        }
        if(count_of_next_symbols != 0 and !was_word_read){
            TmpKey.push_back(it);
            count_of_next_symbols--;
            if(count_of_next_symbols == 0 ){
                was_word_read = true;
                continue;

            }
        }
        if(count_of_next_symbols != 0 and was_word_read){
            TmpValue.push_back(it);
            count_of_next_symbols--;
            if(count_of_next_symbols == 0 ){
                was_word_read = false;
                my_map[TmpKey] = TmpValue;
                TmpKey.clear();
                TmpValue.clear();
                continue;
            }
        }

    }

    file.close();
    WriteParseDataToFile();

    return 0;
}

void Parser::WriteParseDataToFile(){
    string tmp_string = FILE_TO_WRITE;
    tmp_string += "/";
    tmp_string += GetNameFile();
    chmod(tmp_string.c_str(), S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);
    ofstream outfile(tmp_string);
    for (auto it : my_map)
        outfile << it.first << "   " << it.second << endl;


    CopyDFile();
    outfile.close();
}

string Parser::GetNameFile(){
    int count = 0;
    std::string NameFile;
    for(auto it : FileToParse){
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

    string path_to_d_file = string(PATH_TO_CUPS) + string("/") + d_file;

    chmod(path_to_d_file.c_str(), S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);
    ifstream file_in(path_to_d_file, std::ios::binary);
    string out_d_file = string(FILE_TO_WRITE) + string("/") + d_file;
    chmod(out_d_file.c_str(), S_IRWXU | S_IXGRP | S_IWGRP | S_IRGRP | S_IXOTH | S_IWOTH | S_IROTH);
    ofstream file_out(out_d_file, std::ios::binary);
    file_out << file_in.rdbuf();

    file_out.close();
    file_in.close();

}