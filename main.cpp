#include "Watcher.h"
#include "Parser.h"
#include <thread>

int main() {
    ThreadSafeQueue queue;
    Watcher watcher("/var/spool/cups", queue);
    Parser parser("/var/spool/DataFromPrinters", queue);

    std::thread first([&watcher](){ watcher.StartWatch(); });
    std::thread second([&parser](){ parser.StartParse(); });

    first.join();
    second.join();



    return 0;
}