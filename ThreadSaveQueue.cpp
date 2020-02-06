
#include "ThreadSaveQueue.h"

using std::string;

ThreadSafeQueue::ThreadSafeQueue() : frontValue("") {

}

string & ThreadSafeQueue::RetrieveAndDelete(){
    static string tmp = "";
    std::lock_guard<std::mutex> lg(m);
    if( !rawQueue.empty() ) {
        frontValue = rawQueue.front();
        rawQueue.pop();
    }
    return frontValue;
}

void ThreadSafeQueue::Push(std::string val){
    std::lock_guard<std::mutex> lg(m);
    rawQueue.push(val);
}

