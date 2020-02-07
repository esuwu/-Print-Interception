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
#include <queue>


constexpr int maxEvents = 1024;
constexpr int  lenName = 16;
constexpr int  eventSize = sizeof (struct inotify_event);
constexpr int bufLen = maxEvents * ( eventSize + lenName);

class Watcher{
public:
    Watcher(const std::string & _DirToWatch, ThreadSafeQueue & _queue);
    ~Watcher();
    void StartWatch();
private:
    void Watch();
    std::string GetFilePath();
    const std::string DirToWatch;
    int wd;
    int fd;
    char buffer[bufLen];
    std::string FileToParse;
    int CountCreatingFiles;
    ThreadSafeQueue & queue;
};


#endif //WATCHER_H
