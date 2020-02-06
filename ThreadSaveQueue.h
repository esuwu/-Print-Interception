
#ifndef THREADSAVEQUEUE_H
#define THREADSAVEQUEUE_H

#include <mutex>
#include <queue>


class ThreadSafeQueue {
    std::string frontValue;
    std::queue<std::string> rawQueue;
    std::mutex m;
public:
    ThreadSafeQueue();
    ~ThreadSafeQueue() = default;
    std::string & RetrieveAndDelete();
    void Push(std::string val);
};

#endif //THREADSAVEQUEUE_H
