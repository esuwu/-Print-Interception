cmake_minimum_required(VERSION 3.15)
project(PrinterCheck)

set(CMAKE_CXX_STANDARD 17)

include_directories(

)

add_executable(PrinterCheck main.cpp Parser.cpp Watcher.cpp ThreadSaveQueue.cpp)

target_link_libraries(PrinterCheck pthread -lstdc++fs)