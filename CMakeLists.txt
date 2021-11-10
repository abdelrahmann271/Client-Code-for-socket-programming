cmake_minimum_required(VERSION 3.19)
project(client)
link_libraries(ws2_32 wsock32)
set(CMAKE_CXX_STANDARD 14)

add_executable(client main.cpp http_parser.h file_parser.h)