#include "Watcher.h"
#include "Parser.h"
#include <thread>





int main() {
    threadSafe_queue queue;
    Watcher watcher("/var/spool/cups", queue);
    Parser parser(queue);

    std::thread first([&watcher](){ watcher.Watch(); });
    std::thread second([&parser](){ parser.StartParse(); });


    first.join();
    second.join();



    return 0;
}