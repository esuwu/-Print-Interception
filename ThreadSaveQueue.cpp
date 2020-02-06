
#include "ThreadSaveQueue.h"

using std::string;

string & threadSafe_queue::retrieve_and_delete(){

    std::lock_guard<std::mutex> lg(m);
    if( !rawQueue.empty() ) {
        front_value = rawQueue.front();
        rawQueue.pop();
    }
    return front_value;
}

void threadSafe_queue::push(std::string val){
    std::lock_guard<std::mutex> lg(m);
    rawQueue.push(val);
}
