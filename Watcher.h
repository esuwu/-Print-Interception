//
// Created by alexandr on 2/6/20.
//

#ifndef WATCHER_H
#define WATCHER_H

#include <iostream>
#include <string>
#include <sys/inotify.h>
#include <unistd.h>
#include "ThreadSaveQueue.h"


#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE  (sizeof (struct inotify_event))
#define BUF_LEN     (MAX_EVENTS * ( EVENT_SIZE + LEN_NAME))



class Watcher{
public:
    Watcher(const std::string & _DirToWatch, threadSafe_queue & _queue);
    ~Watcher();
    void Watch();
private:
    std::string GetFilePath();
    const std::string DirToWatch;
    int wd;
    int fd;
    char buffer[BUF_LEN];
    std::string FileToParse;
    int CountCreatingFiles;
    threadSafe_queue & queue;
};


#endif //WATCHER_H
