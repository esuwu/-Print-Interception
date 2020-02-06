
#ifndef THREADSAVEQUEUE_H
#define THREADSAVEQUEUE_H

#include <mutex>
#include <queue>


class threadSafe_queue {


    std::queue<std::string> rawQueue;
    std::mutex m;
    std::string front_value;


public:
    threadSafe_queue() = default;
    ~threadSafe_queue() = default;
    std::string & retrieve_and_delete();
    void push(std::string val);
};

#endif //THREADSAVEQUEUE_H
