
#include "Watcher.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

Watcher::Watcher(const string & _DirToWatch, ThreadSafeQueue & _queue) : queue(_queue), DirToWatch(_DirToWatch), CountCreatingFiles(0){

}

Watcher::~Watcher() {
    inotify_rm_watch( fd, wd );
    close( fd );
}

void Watcher::StartWatch(){
    std::cout << "Watching :: " << DirToWatch << endl;
    while(true){
        Watch();
    }
}

void Watcher::Watch(){
    fd = inotify_init();
    if(fd < 0){
        std::cerr << "Couldn't initialize inotify" << endl;
        return;
    }
    wd = inotify_add_watch(fd, DirToWatch.c_str(), IN_CREATE );
    if(wd == -1){
        std::cerr << "Couldn't add watch" << endl;
        return;
    }

    int length = read(fd, buffer, bufLen);
    if (length < 0){
        std::cerr << "File was not read" << endl;
        return;
    }

    int i = 0;
    while ( i < length ) {
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
        if ( event->len ) {
            if ( event->mask & IN_CREATE) {
                if (event->mask & IN_ISDIR)
                    cout << "The directory " << event->name << "was created" << endl;
                else{
                    cout << "The file " << event->name << " was created with WD " << event->wd << endl;
                    FileToParse = event->name;
                    GetFilePath();
                }

            }
            i += eventSize + event->len;
        }
    }
}

string Watcher::GetFilePath() {
    string clear_string;
    if(FileToParse[0] == 'c') {
        for (auto it : FileToParse) {
            if (it == '.') {
                break;
            }
            clear_string.push_back(it);
        }
        queue.Push(DirToWatch + "/" + clear_string);
        return clear_string;
    }
    return "";

}
